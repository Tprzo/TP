# TP
// Partie 1 : TCP et UDP : Pour compiler les 3 fichiers : gcc tsock.c -o tsock Pour lancer le programme il faut lancer le puits en premier : ./tsock -p localhost 8000 ( TCP ) et ./tsock -p -u localhost 8000 puis lancer la source : ./tsock -s localhost 8000 ( TCP ) et ./tsock -s -u localhost 8000 le -n permet de donner le nombres de messages

// Partie 2 : BAL

Il faut lancer 3 terminaux :

1)BAL SERVEUR : ./tsock -b 8000
2)BAL EMMETEUR : ./tsock -e# [IP ADRESS] 8000
3)BAL RECEPTEUR : ./tsock -r# localhost 8000
