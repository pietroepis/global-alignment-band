# global-alignment-band

**Input**\
L'input proviene dal file `input.txt`, che deve rispettare la seguente struttura:\
l1 s1 l2 s2\
I valori devono essere separati da uno spazio, e hanno il seguente significato:
- l1: Lunghezza della prima stringa
- s1: La prima stringa (lunga l1 caratteri)
- l2: Lunghezza della seconda stringa
- s2: La seconda stringa (lunga l2 caratteri)

I caratteri ammessi per s1 e s2 sono: 'A', 'R', 'N', 'D', 'C', 'Q', 'E', 'G', 'H', 'I', 'L', 'K', 'M', 'F', 'P', 'S', 'T', 'W', 'Y', 'V', 'B', 'Z' e 'X'

**Output**\
L'output viene scritto nel file ` output.txt`\
Il file contiene il valore dell'allineamento e la rappresentazione di s1 e s2 allineate (`-` rappresenta un indel)
