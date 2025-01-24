import pytest
import time
import matplotlib.pyplot as plt
import timeit

def eval(smot, cmot) :
    
    for j in range(len(cmot)):
        if cmot[j] == smot[j]:
            smot[smot.index(cmot[j])] = "V"
            cmot[j] = "V"
    for j in range(len(cmot)):
        if  cmot[j] in smot:
            if cmot[j] != "V"  and smot[smot.index(cmot[j])] != "V" :
                smot[smot.index(cmot[j])] = "J"
                cmot[j] = "J"
                
            
    for j in range(len(cmot)):
        if cmot[j] != "J" and cmot[j] != "V":
            cmot[j] = "G"

    return cmot

def perform():
    time1 = timeit.timeit("""eval(["M", "O", "T"],["P", "A", "S"])""", setup="""from __main__ import eval""", number=1000)
    time2 = timeit.timeit("""eval(["P", "E", "A", "U"],["P", "E", "A", "U"])""", setup="""from __main__ import eval""", number=1000)
    time3 = timeit.timeit("""eval(["P", "R", "E", "N","D"],["P", "A", "U", "S","E"])""", setup="""from __main__ import eval""", number=1000)
    time4 = timeit.timeit("""eval(["T", "R", "A", "Q", "U", "E", "R"],["P", "R", "I", "E", "R", "E", "S"])""", setup="""from __main__ import eval""", number=1000)
    time5 = timeit.timeit("""eval(["C", "O", "N", "S", "T", "I", "T", "U", "T", "I", "O", "N"],["C", "O", "N", "S", "T", "I", "T", "U", "T", "I", "O", "N"])""", setup="""from __main__ import eval""", number=1000)
    times = []
    lens = []
    lens.append(3)
    lens.append(4)
    lens.append(5)
    lens.append(7)
    lens.append(12)
    times.append(time1)
    times.append(time2)
    times.append(time3)
    times.append(time4)
    times.append(time5)
    
    plt.plot(lens, times)
    plt.ylabel('temps_execution')
    plt.xlabel('longueurs_liste')

    plt.show()
    #print(times)
    
    
    
    
    
    
    
"""""
def remplaceString(chaineInitiale: str, nouveauCaractere: str, indice: int):
    #debut = time.time()
    chaineFinale = ""
    for i in range(len(chaineInitiale)) :
        if i != indice :
            chaineFinale += chaineInitiale[i]
        else :
            chaineFinale += nouveauCaractere
    #print("rps", time.time() - debut)
    return chaineFinale


## Fonction evaluation()

def evaluation(motJoueur: str, motSecret: str) -> str:
    
    Évalue le mot soumis par le joueur en fonction du mot à deviner.
    motJoueur : mot soumis par le joueur
    motSecret : mot à deviner

    Retourne une chaîne de caractères correspondant aux couleurs à afficher.
    G -> la lettre n'est pas dans le mot
    J -> la lettre est dans le mot, mais MAL placée
    V -> la lettre est dans le mot, mais BIEN placée

    Cette fonction ne vérifie pas si les paramètres passés en argument sont valides.
    

    debut = time.time()
    motJoueur, motSecret = motJoueur.lower(), motSecret.lower()
    
    
    
    if len(motJoueur) != len(motSecret) : # Les mots doivent être de même taille
        return "ErreurMotJoueurTailleDifferente"
    n = len(motSecret)
    vert, jaune = [0] * n, [0] * n
    # vert = indices des lettres BIEN placées dans le mot motJoueur
    # jaune = indices des lettres MAL placées dans le mot motJoueur

    # Si une lettre est correctement placée dans motJoueur (même indice dans motJoueur et motSecret), on incrémente la valeur de vert[i] où i est l'indice de la lettre bien placée.
    for i in range(n) :
        if motJoueur[i] == motSecret[i] :
            vert[i] += 1

    # On copie motJoueur et motSecret pour effectuer les manipulations suivantes sans altérer les paramètres d'entrée
    motJoueurCopie, motSecretCopie = list(motJoueur), list(motSecret)

    # Si une lettre est bien placée dans le mot du joueur passé en entrée de la fonction, on la supprime dans motJoueurCopie et motSecretCopie puisqu'elle a été "utilisée"
    for i in range(n) :
        if vert[i] > 0 :
            motJoueurCopie[i] = "#"
            motSecretCopie[i] = "#"
    motJoueurCopie = "".join(motJoueurCopie)
    motSecretCopie = "".join(motSecretCopie)

    #print("Après suppression (si lettres bien placées) : ", motJoueurCopie, motSecretCopie, end = "\n\n")

    # On regarde, pour chaque lettre de motJoueurCopie, si elle est égale à une lettre de motSecretCopie et si leurs indices sont différents (position différente). Si c'est le cas, alors on incrémente jaune[i] pour indiquer que la i-ème lettre de motJoueurCopie est mal placée.
    # On supprime ensuite les deux lettres utilsiées (une dans motJoueurCopie et une dans motSecretCopie)
    for i in range(n) :
        for j in range(n) :
            if motJoueurCopie[i] != "#" and motJoueurCopie[i] == motSecretCopie[j] and i != j :
                #print(motJoueurCopie[i], "|", motJoueur, i, "|", motSecret, j, "|", motJoueurCopie, "|", motSecretCopie)
                jaune[i] += 1
                #print("Liste jaune :", jaune)
                motJoueurCopie = remplaceString(motJoueurCopie, "#", i)
                motSecretCopie = remplaceString(motSecretCopie, "#", j)
                #print("Après suppression des lettres utilisées :", motJoueurCopie, motSecretCopie, end = "\n\n")

    # Avec les listes vert et jaune, on en déduit la couleur de chaque lettre
    resultat = ""
    for i in range(n) :
        evalLettre = ""
        if jaune[i] > 0 :
            evalLettre = "J"
        if vert[i] > 0 :
            evalLettre = "V"
        if (evalLettre != "J") and (evalLettre != "V") :
            evalLettre = "G"
        resultat += evalLettre
        
    #print("Évaluation du mot :", resultat)
    #print("Temps : ", time.time() - debut, end = "\n\n+++++\n\n")
    return resultat



    # Exemple : remplaceString("GGGGG", "V", 2) -> "GGVGG"

## Tests

assert evaluation("examen", "utopie") == "JGGGGG"
assert evaluation("gueule", "gorgeu") == "VJJGGG"
assert evaluation("arizona", "zmpoozm") == "GGGJVGG"
assert evaluation("rouleau", "boucler") == "JVVJJGG"
assert evaluation("etablir", "etablri") == "VVVVVJJ"
assert evaluation("azerty", "azerty") == "VVVVVV"
assert evaluation("azerty", "zertya") == "JJJJJJ"
assert evaluation("pipopipopi", "piperaient") == "VVVGGJGGGG"
assert evaluation("ACCOMPAGNATRICE", "ORTHOGRAPHIERAS") == "JGGJGJJJGGJJJGJ"
assert evaluation("ACCOMPAGNATRICE", "ANTICYCLONIQUES") == "VJJJGGGGJGJGJGJ"

#assert evaluation("", "") == ""

#print("Voir ligne 53 si vous avez un souci de compréhension de l'affichage")

"""""
if __name__=="__main__":
    #print(eval(["T", "R", "A", "Q", "U", "E", "R"],["P", "R", "I", "E", "R", "E", "S"]))
    perform()