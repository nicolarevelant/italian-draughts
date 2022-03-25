italian-draughts(1)

# NAME
italian-draughts - Gioco della Dama Italiana

# SYNOPSIS
italian-draughts [options...]

# OPTIONS
## -h, --help
Mostra il messaggio di aiuto ed esce

## -l, --locale
Imposta la lingua

## -t, --theme
Imposta il tema

# DESCRIPTION
Questo programma ti permette di giocare alla Dama Italiana, un gioco da tavolo
italiano con 2 giocatori ed una scacchiera 8x8. 

# CONFIGURATION
italian-draughts cerca le risorse nei seguenti percorsi:

. /etc/italian-draughts
. ~/.config/italian-draughts

Esse vengono lette in cascata dal primo percorso all'ultimo, ovvero a partire
da un insieme vuoto ogni singola risorsa viene aggiunta all'insieme oppure
viene aggiornato il suo valore.

Per ulteriori dettagli vedere *italian-draughts*(5).

# SEE ALSO
*italian-draughts*(5)

# AUTHORS
Maintained by Nicola Revelant <nicolarevelant44@gmail.com>. Up-to-date sources
can be found at https://gitlab.com/nicolarevelant44/italian-draughts and
bugs/patches can be submitted by e-mail.
