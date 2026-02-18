import subprocess
import os
import sys

# Vérifie si le répertoire "build" existe
if not os.path.exists("build"):
    os.makedirs("build")

# Changement de répertoire vers le dossier "build"
os.chdir("build")

traceLog = '1'
# Définition du nombre de noeuds
NB_NODES = 2
if len(sys.argv) > 1:
    for i in range (1, len(sys.argv)):
        arg = sys.argv[i]
        if (arg == "-n"):
            NB_NODES = int(sys.argv[1 + 1])
        if(arg == "-t"):
            traceLog = '1'
            
# Définition des commandes restantes
qt_path = "/Users/adjysedardesir/Qt/6.10.1/macos"

commands = [
    f"cmake ../.. -DCMAKE_TOOLCHAIN_FILE=/Users/adjysedardesir/Documents/Dev/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_PREFIX_PATH={qt_path} -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH",
    "make"
]

# Exécution des commandes
for command in commands:
    subprocess.run(command, shell=True)

comm = './bin/NetworkPokemon-exemple'

commands = list()
# Exécution des commandes
for i in range(NB_NODES):
    commands.append(subprocess.Popen([ comm, '-d', '../../data/peer_' + str(i + 1) + '/', '-l', 'nodes.txt', '-t', traceLog]))

for i in range(len(commands)):
    commands[i].wait()
