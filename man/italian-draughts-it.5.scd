italian-draughts(5)

# NAME
italian-draughts - file di risorse

# DESCRIPTION
italian-draughts cerca le risorse nei seguenti percorsi:

. /etc/italian-draughts
. ~/.config/italian-draughts

Esse vengono lette in cascata dal primo percorso all'ultimo, ovvero a partire
da un insieme vuoto ogni singola risorsa viene aggiunta all'insieme oppure
viene aggiornato il suo valore.

Non vengono caricate tutte le risorse in memoria, bensì quelle richieste al
programma (vedere *italian-draughts*(1)) e quelle del genitore in modo
ricorsivo fino ad arrivare a quelle predefinite.

In generale le risorse si trovano in <percorso>/<tipo>/<modificatore>,
con <percorso> il percorso delle risorse, <tipo> il tipo di risorsa
(es. strings oppure colors) e <modificatore> il file in cui ci sono alcune
risorse.

Per supportare diverse configurazioni (es. diverse lingue, diversi temi)
le risorse di uno specifico tipo sono suddivise in 1 o più file.

Per ogni tipo di risorsa, tutti i valori predefiniti si trovano nel file
"default", il quale dovrebbe essere sempre presente in ogni directory <tipo>.

# MODIFICATORI
I modificatori permettono di creare delle varianti di uno specifico tipo
(es. localizzare un programma, applicare un tema diverso, etc.). Qui di seguito
viene indicato quale modificatore usare (ovvero il nome del file) per i tipi
di risorse più utilizzate, e fra parentesi la gerarchia utilizzata nel caso
in cui il modificatore non sovrascriva la risorsa del suo genitore. I caratteri
non validi sono: "/" e "\\".

## strings (default > language > language_country)
Il codice ISO della lingua (es. en_US) tenendo conto che l'eventuale presenza
di un file con codice ISO che include il Paese è figlio di quello in cui
include solo la lingua e dunque quest'ultimo, il tal caso, deve essere
presente. Nel caso in cui la lingua richiesta contenga il charset
(es. en_US.UTF-8) quest'ultimo verrà ignorato (il programma utilizza
esclusivamente UTF-8).

## colors (default > theme)
Il nome del tema. Non è prevista alcuna gerarchia specifica.

# SEE ALSO
*italian-draughts*(1)

# AUTHORS
Maintained by Nicola Revelant <nicolarevelant44@gmail.com>. Up-to-date sources
can be found at https://gitlab.com/nicolarevelant44/italian-draughts and
bugs/patches can be submitted by e-mail.
