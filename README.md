# Network Pokemon (Application Pair-à-Pair de Partage d’Images Pokémon)

L’objectif de cette application est de créer un réseau pair-à-pair (P2P) permettant le partage d’images de Pokémon. Chaque nœud du réseau joue à la fois le rôle de client et de serveur.

## Fonctionnalités

- Afficher la liste des serveurs connus par le nœud
- Afficher la liste des images de Pokémon disponibles sur le nœud
- Demander une image particulière

## Prérequis

- C++ doit être installé sur votre système.
- Python ou Python 3 doit être installé sur votre système.

## Comment utiliser

1. Clonez ce dépôt sur votre machine.

2. Vous pouvez maintenant exécuter l'application de l'une des manières suivantes en utilisant un terminal ou une invite de commande :
    
    - Dans un terminal, lancer 30 nœuds et 1 nœud pour afficher et télécharger des images:  
      - Pour lancer 30 nœuds avec les traces si nécessaire :
        ```bash
        python3 execute.py -n 30 -t
        ```

      - Ensuite, dans un deuxième terminal, rendez-vous dans le dossier "build" et lancez le nœud 31 sans les traces pour tester les fonctionnalités :
        ```bash
        ./src/main -d ../data/peer_31/ -l nodes.txt
        ```
    - Pour lancer N nœuds:
     ```bash
     # -----
     # python3 execute.py -n N -t 
     # ----
     # -n N : le nombre de nœuds à lancer (0 < N <= 31) 
     # -t : pour activer les journaux et traces
     # Exemple pour lancer 30 nœuds :
     python3 execute.py -n 30 -t
     ```
