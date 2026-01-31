
set(MSVC_WARNINGS
	/WX
	/W4
)

set(MSVC_RELEASE_OPTIMISATION
	/O2								# Optimisation maximale de code en Release (favorise la vitesse)
)

set(MSVC_DEBUG_OPTIMISATION
	/Zi								# Génère les informations de débogage dans un fichier PDB
	/Od								# Désactive les optimisations, utile pour déboguer
)

set(GCC_WARNINGS
	-Werror							# Traite tous les warnings comme des erreurs
	-Wall							# Active un ensemble standard d’avertissements courants
	-Wextra							# Active des avertissements supplémentaires non inclus dans -Wall
	-Wpedantic						# Respect strict du standard C++ (avertit sur les extensions non standard)
	-Wconversion					# Avertit sur les conversions implicites (ex: float → int)
	-Wshadow						# Avertit si une variable masque une autre (ex: variable locale cache un champ membre)
	-Wnon-virtual-dtor				# Avertit si une classe polymorphe n’a pas de destructeur virtual
    -Wcast-align					# Avertit sur des casts mal alignés (ex: pointer une structure mal alignée)
    -Wunused						# Avertit pour les variables, fonctions, paramètres non utilisés
    -Woverloaded-virtual			# Avertit si une méthode masque une autre avec le même nom mais signature différente
    -Wsign-conversion				# Avertit sur les conversions de signed ↔ unsigned
    -Wdouble-promotion				# Avertit si float est promu en double inutilement
    -Wformat=2						# Avertit sur mauvais formats dans printf/scanf (plus strict que -Wformat)
    -Wimplicit-fallthrough			# Avertit si un case de switch tombe dans un autre sans break
    -Wsuggest-override				# Suggère override pour les fonctions virtuelles redéfinies
    -Wnull-dereference				# Avertit sur les déréférencements potentiels de pointeurs nuls
    -Wold-style-cast				# Avertit si tu utilises un cast C-style ((int)x) au lieu de static_cast, etc
	-Wmissing-field-initializers	# Une structure ou classe agrégée est partiellement initialisée via une initialisation entre accolades ({})
)

set(GCC_RELEASE_OPTIMISATION
	-O3								# Optimisation agressive pour la performance maximale
)

set(GCC_DEBUG_OPTIMISATION
	-g								# Génère des informations de débogage (DWARF) utilisables par gdb/lldb
)

add_subdirectory(NetworkPokemonUi)
