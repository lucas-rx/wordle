import os
import time
print(os.getcwd())

def extract_dico():
    debut = time.time()
    file = "../dictionary/ods/ods6.txt"

    reading = open(file, "r")
    scan = reading.readlines()
    reading.close()

    for i in range(len(scan)) :
        scan[i] = scan[i][:-1]

    # Récupération de la longueur minimum et maximum d'un mot 
    # mini = 2, maxi = 15
    """
    mini, maxi = 1000 * 1000, 0
    for i in range(len(scan)) :
        if len(scan[i]) < mini :
            mini = len(scan[i])
        if len(scan[i]) > maxi :
            maxi = len(scan[i])
    print(mini, maxi)
    """

    all_data = [[] for i in range(14)]

    for i in range(len(scan)) :
        n = len(scan[i])
        all_data[n - 2].append(scan[i])

    c = 0 # Nombre de mots
    for i in range(2, 16) :
        filename = "../dictionary/dico" + str(i) + "lettres.txt"
        writing = open(filename, "w")
        for j in range(len(all_data[i - 2])) :
            if j == len(all_data[i - 2]) - 1 :
                writing.write(all_data[i - 2][j])
            else :
                writing.write(all_data[i - 2][j] + "\n")
            c += 1
        writing.close()
    print(time.time() - debut)
    print(c)

def script() :
    print("write 'yes' to execute")
    question = input()
    if question == "yes" :
        extract_dico()
    else :
        print("no-op")

#script()