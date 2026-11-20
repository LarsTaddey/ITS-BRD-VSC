Option Explicit

' MSCOMM32.OCX must be installed, registered and usable by Windows Script Host.
' MSComDlg is optional: XP's UserAccounts dialog can select files one at a time.
Const FRAME_SIZE = 512
Const ERR_BASE = &H80040400
Dim comm, hexNode
Set comm = Nothing

On Error Resume Next
SendFiles
If Err.Number <> 0 Then
    MsgBox "Send failed: " & Err.Description, vbCritical, "Send bitmap files"
End If
If Not (comm Is Nothing) Then
    If comm.PortOpen Then comm.PortOpen = False
End If
On Error GoTo 0

Sub SendFiles()
    Dim files, portName, portNumber, xml, fileName, command, pending
    files = SelectFiles()
    If Not IsArray(files) Then Exit Sub

    portName = UCase(Trim(InputBox("Enter the COM port, for example COM3:", "Send bitmap files", "COM3")))
    If portName = "" Then Exit Sub
    If Left(portName, 3) <> "COM" Or Not IsNumeric(Mid(portName, 4)) Then
        Err.Raise ERR_BASE + 1, , "Enter a COM port such as COM3."
    End If
    portNumber = CDbl(Mid(portName, 4))
    If portNumber < 1 Or portNumber > 255 Or portNumber <> Fix(portNumber) Then
        Err.Raise ERR_BASE + 1, , "Invalid COM port number."
    End If

    Set xml = CreateObject("MSXML2.DOMDocument")
    Set hexNode = xml.createElement("bytes")
    hexNode.dataType = "bin.hex"
    Set comm = OpenComm()
    comm.CommPort = CInt(portNumber)
    comm.Settings = "115200,N,8,1"
    comm.Handshaking = 0
    comm.InputMode = 1
    comm.InputLen = 1
    comm.OutBufferSize = 4096
    comm.DTREnable = True
    comm.RTSEnable = True
    comm.PortOpen = True
    comm.InBufferCount = 0
    comm.OutBufferCount = 0

    ConnectBoard
    pending = ""
    For Each fileName In files
        If pending = "" Then
            command = ReadCommand()
        Else
            command = pending
            pending = ""
        End If
        Do While command = "D"
            WaitForOutput
            SendHex "0000"
            command = ReadCommand()
        Loop
        If command = "S" Then
            pending = SendFileWithFlowControl(fileName)
        Else
            SendFileWithoutFlowControl fileName
        End If
    Next
    WaitForOutput
    comm.PortOpen = False
    MsgBox "All files sent.", vbInformation, "Send bitmap files"
End Sub

Function OpenComm()
    Dim obj, description
    On Error Resume Next
    Set obj = CreateObject("MSCOMMLib.MSComm")
    If Err.Number <> 0 Then
        description = Err.Description
        On Error GoTo 0
        Err.Raise ERR_BASE + 2, , "Cannot create MSComm. MSCOMM32.OCX must be installed, registered and licensed for scripting. " & description
    End If
    On Error GoTo 0
    Set OpenComm = obj
End Function

Function SelectFiles()
    Dim dialog, selection, parts, files(), i, count, fso, answer
    Set fso = CreateObject("Scripting.FileSystemObject")
    Set dialog = Nothing
    On Error Resume Next
    Set dialog = CreateObject("MSComDlg.CommonDialog")
    Err.Clear
    On Error GoTo 0

    If Not (dialog Is Nothing) Then
        dialog.DialogTitle = "Select bitmap files"
        dialog.Filter = "Bitmap files (*.bmp)|*.bmp"
        dialog.Flags = &H80000 Or &H200 Or &H1000 Or &H800 Or &H4
        dialog.MaxFileSize = 32767
        dialog.CancelError = False
        dialog.ShowOpen
        selection = dialog.FileName
        If selection = "" Then Exit Function
        parts = Split(selection, Chr(0))
        If UBound(parts) = 0 Then
            SelectFiles = Array(parts(0))
        ElseIf parts(1) = "" Then
            SelectFiles = Array(parts(0))
        Else
            count = 0
            For i = 1 To UBound(parts)
                If parts(i) <> "" Then
                    ReDim Preserve files(count)
                    files(count) = fso.BuildPath(parts(0), parts(i))
                    count = count + 1
                End If
            Next
            If count > 0 Then SelectFiles = files
        End If
    Else
        ' This XP dialog does not support multiselect; repeat it for more files.
        Set dialog = CreateObject("UserAccounts.CommonDialog")
        dialog.Filter = "Bitmap files (*.bmp)|*.bmp"
        count = 0
        Do
            If Not dialog.ShowOpen Then Exit Do
            ReDim Preserve files(count)
            files(count) = dialog.FileName
            count = count + 1
            answer = MsgBox("Add another bitmap file?", vbYesNo + vbQuestion, "Send bitmap files")
        Loop While answer = vbYes
        If count > 0 Then SelectFiles = files
    End If
End Function

Sub ConnectBoard()
    Dim i, ready
    Do
        SendHex "48"  ' H
        ready = False
        For i = 1 To 100
            If comm.InBufferCount > 0 Then
                ready = True
                Exit For
            End If
            WScript.Sleep 10
        Next
    Loop Until ready
    If ReadByte() <> Asc("R") Then
        Err.Raise ERR_BASE + 3, , "Expected R from the board. Restart the board and try again."
    End If
    SendHex "47"  ' G
End Sub

Function ReadByte()
    Dim bytes, value
    Do While comm.InBufferCount = 0
        WScript.Sleep 10
    Loop
    bytes = comm.Input
    For Each value In bytes
        ReadByte = CInt(value)
        Exit Function
    Next
    Err.Raise ERR_BASE + 4, , "Failed to read a byte from the COM port."
End Function

Function ReadCommand()
    Dim command
    command = Chr(ReadByte())
    If command <> "D" And command <> "S" And command <> "F" Then
        Err.Raise ERR_BASE + 5, , "Unexpected command from the board: " & Asc(command)
    End If
    ReadCommand = command
End Function

Function SendFileWithFlowControl(fileName)
    Dim stream, count, command
    Set stream = OpenBinaryFile(fileName)
    Do
        count = NextChunkSize(stream)
        SendFrame stream, count
        command = ReadCommand()
    Loop While command = "D"
    stream.Close
    SendFileWithFlowControl = command
End Function

Sub SendFileWithoutFlowControl(fileName)
    Dim stream, count
    Set stream = OpenBinaryFile(fileName)
    Do
        count = NextChunkSize(stream)
        SendFrame stream, count
    Loop While count = FRAME_SIZE
    ' Match the Python sender's additional EOF frame, including for empty files.
    SendFrame stream, 0
    stream.Close
End Sub

Function OpenBinaryFile(fileName)
    Dim stream
    Set stream = CreateObject("ADODB.Stream")
    stream.Type = 1
    stream.Open
    stream.LoadFromFile fileName
    Set OpenBinaryFile = stream
End Function

Function NextChunkSize(stream)
    Dim remaining
    remaining = stream.Size - stream.Position
    If remaining > FRAME_SIZE Then remaining = FRAME_SIZE
    NextChunkSize = remaining
End Function

Sub SendFrame(stream, count)
    WaitForOutput
    SendHex Right("0" & Hex(count \ 256), 2) & Right("0" & Hex(count Mod 256), 2)
    If count > 0 Then comm.Output = stream.Read(count)
End Sub

Sub SendHex(value)
    hexNode.Text = value
    comm.Output = hexNode.nodeTypedValue
End Sub

Sub WaitForOutput()
    Do While comm.OutBufferCount > 0
        WScript.Sleep 10
    Loop
End Sub