import subprocess
import os
import sys

# Vérifie si le répertoire "build" existe
if not os.path.exists("build"):
    os.makedirs("build")

# Changement de répertoire vers le dossier "build"
os.chdir("build")

traceLog = '0'
# Définition du nombre de noeuds
NB_NODES = 31
if len(sys.argv) > 1:
    for i in range (1, len(sys.argv)):
        arg = sys.argv[i]
        if (arg == "-n"):
            NB_NODES = int(sys.argv[1 + 1])
        if(arg == "-t"):
            traceLog = '1'
            
# Définition des commandes restantes
commands = [
    "cmake ..",
    "make",
]

# Exécution des commandes
for command in commands:
    subprocess.run(command, shell=True)

comm = './src/main'

commands = list()
# Exécution des commandes
for i in range(NB_NODES):
    commands.append(subprocess.Popen([ comm, '-d', '../data/peer_' + str(i + 1) + '/', '-l', 'nodes.txt', '-t', traceLog]))

for i in range(len(commands)):
    commands[i].wait()
