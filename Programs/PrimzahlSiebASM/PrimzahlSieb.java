public class PrimzahlSieb
{
    public static void main(String[] args)
    {
        sieb();

    }

    private static final int _NUMBER = 1000;
    private static final boolean[] _PRIMZAHL_SIEB = new boolean[_NUMBER + 1];
    private static final int _START_PRIMZAHL = 2;

    private static void sieb()
    {
        // Alle Feldelemente ab 2 auf 1 (wahr) setzen
        for (int i = _START_PRIMZAHL; i <= _NUMBER; ++i)
        {
            _PRIMZAHL_SIEB[i] = true;
        }

        // Finde die nächste potenzielle Primzahl
       for(int i = _START_PRIMZAHL; i <= _NUMBER; ++i)
       {
           // Wenn das Quadrat groesser als das Limit, dann abbrechen
           if(i * i > _NUMBER)
           {
               break;
           }

           // Wenn die nächste Primzahl gefunden wurde, also true dann markiere das Quadrat und alle vielfachen
           if(_PRIMZAHL_SIEB[i])
           {
               for(int j = i * i; j <= _NUMBER; j += i)
               {
                _PRIMZAHL_SIEB[j] = false;
               }
           }
       }
        // Wenn alle Vielfachen markiert wurden, sind nur noch Primzahlen übrig
        // Jetzt wird die Methode Abspeichern aufgerufen, um die Primzahlen in einem eigenem Feld zu speichern
       abspeichern();
    }

    private static void abspeichern()
    {
        int count = 0;
        // Die Groesse des neuen Feldes bestimmen, anhand der gefundenen Werte, die =true sind
        for (int i = 0; i <= _NUMBER; ++i)
        {
            if (_PRIMZAHL_SIEB[i])
            {
                ++count;
            }
        }
        int[] primZahlen = new int[count];

        // Zähler wieder auf 0 setzen
        count = 0;
        // Gefundene Primzahlen in ein neues Feld schreiben
        for( int i = _START_PRIMZAHL; i <= _NUMBER; ++i)
        {
            if (_PRIMZAHL_SIEB[i])
            {
                primZahlen[count] = i;
                ++count;
            }
        }
    }
}
