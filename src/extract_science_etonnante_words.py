import os

def extract_ods(length) :
    file = "../dictionary/ods/dico" + str(length) + "lettres.txt"
    eye = open(file, "r")
    fileContent = eye.readlines()
    eye.close()

    for i in range(len(fileContent)) :
        fileContent[i] = fileContent[i].strip()
        fileContent[i] = fileContent[i].upper()
    fileContent.sort()
    print("ods :", fileContent[0])
    return fileContent

def extract_science_etonnante(length) :
    file = "../dictionary/science_etonnante/mots_" + str(length) + ".txt"
    eye = open(file, "r")
    fileContent = eye.readlines()
    eye.close()

    for i in range(len(fileContent)) :
        fileContent[i] = fileContent[i].strip()
        fileContent[i] = fileContent[i].upper()
    fileContent.sort()
    print("SE :", fileContent[0])
    return fileContent

def append_SE_words_to_ods() :
    for i in range(4, 13) :
        c = 0
        ods = extract_ods(i)
        SE = extract_science_etonnante(i)
        new_ods = ods.copy()
        for j in range(len(SE)) :
            if SE[j] not in new_ods :
                c += 1
                #print(j, SE[j])
                new_ods.append(SE[j])
        new_ods.sort()
        print(f"Nombre de nouveaux mots de {i} lettres trouvés : {c}")
        print(f"Longueur de l'ODS de {i} lettres : {len(ods)}")
        print(f"Longueur du newODS de {i} lettres : {len(new_ods)}")
        print("Différence :", c + len(ods) - len(new_ods))

        file = "../dictionary/ods/dico" + str(i) + "lettres.txt"
        pen = open(file, "w")
        for k in range(len(new_ods)) :
            pen.write(new_ods[k] + "\n")
        pen.close()
        print("Success for i =", i)
        
def script() :
    print("Location :", os.getcwd())
    print("Execute extract_science_etonnante_words.py ? Write 'YES' to execute")
    question = input()
    if question == "YES" :
        append_SE_words_to_ods()
        print("Executed !")
    else :
        print("Not executed.")

script()
        