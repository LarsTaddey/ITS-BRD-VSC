public class PrimzahlSieb
{
    public static void main(String[] args)
    {
        sieb();
    }

    private static final int LIMIT = 1000;
    private static final boolean[] PRIMZAHL_SIEB = new boolean[LIMIT + 1];

    private static void sieb()
    {
        // Alle Feldelemente ab 2 auf 1 (wahr) setzen
        for (int i = 2; i <= LIMIT; ++i)
        {
            PRIMZAHL_SIEB[i] = true;
        }

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
       abspeichern();
    }

    private static void abspeichern()
    {
        int zaehler = 0;
        // Die Groesse des neuen Feldes bestimmen, anhand der gefundenen Werte, die =true sind
        for (int i = 2; i <= LIMIT; ++i)
        {
            if (PRIMZAHL_SIEB[i])
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
            if (PRIMZAHL_SIEB[i])
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
        abspeichern();
    }
}
