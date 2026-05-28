public class PrimzahlSieb
{
    public static void main(String[] args)
    {
        sieb();
    }

    private static final int LIMIT = 1000;
    private static final boolean[] PRIMZAHL_SIEB = new boolean[LIMIT + 1];

    /**
     * Angepasst für Aufgabe 5. Gibt eine Liste aller Primzahlen als boolean[] zurück, wobei die Indizes der Primzahlen auf true gesetzt sind.
     * @return boolean[] mit true an den Indizes der Primzahlen, false an den anderen Indizes
     */
    private static boolean[] sieb()
    {
        // Alle Feldelemente ab 2 auf 1 (wahr) setzen
        for (int i = 2; i <= LIMIT; ++i)
        {
            PRIMZAHL_SIEB[i] = true;
        }

        // Potentielle Primzahl finden und Vielfache markieren
        for (int i = 2; i * i <= LIMIT; ++i){
            if (PRIMZAHL_SIEB[i])
            {
                for(int j = i * i; j <= LIMIT; j += i)
                {
                    PRIMZAHL_SIEB[j] = false;
                }
            }
        }

        // Alle Vielfachen wurden markiert
       return PRIMZAHL_SIEB;
    }

    /**
     * Diese Methode speichert die gefundenen Primzahlen in einem neuen Feld ab und gibt dieses aus. Sie wird aufgerufen, nachdem das Sieb ausgeführt wurde.
     * @param primzahlen Das boolean[] mit den markierten Primzahlen, das vom Sieb zurückgegeben wurde
     */
    private static void abspeichern(boolean[] primzahlen)
    {
        int zaehler = 0;
        // Die Groesse des neuen Feldes bestimmen, anhand der gefundenen Werte, die =true sind
        for (int i = 2; i <= LIMIT; ++i)
        {
            if (primzahlen[i])
            {
                ++zaehler;
            }
        }
        // Setze die Groesse des Feldes auf den Wert des Zaehlers
        int[] primZahlen = new int[zaehler];

        int index = 0;
        // Gefundene Primzahlen in ein neues Feld schreiben
        for(int i = 2; i <= LIMIT; ++i)
        {
            if (primzahlen[i])
            {
                primZahlen[index] = i;
                ++index;
            }
        }
        System.out.println(java.util.Arrays.toString(primZahlen));
    }

    private static void siebWHILE()
    {
        // Alle Feldelemente ab 2 auf 1 (wahr) setzen
        for (int i = 2; i <= LIMIT; ++i)
        {
            PRIMZAHL_SIEB[i] = true;
        }

        int i = 2;
        while(i*i <= LIMIT)
        {
            if(PRIMZAHL_SIEB[i])
            {
                int j = i * i;
                while(j <= LIMIT)
                {
                    PRIMZAHL_SIEB[j] = false;
                    j += i;
                }
            }
            ++i;
        }        
    }
}
