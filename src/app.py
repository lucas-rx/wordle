from crypt import methods
import json
#from evaluation import evaluation

from pydoc import render_doc
from statistics import mode
from flask import Flask, render_template, request, redirect, session
from flask_session import Session
import random as rd
import sqlite3
import hashlib # Pour hacher un mot de passe
import string # Pour générer un salt
from init_database import init_database # Création de la BD

app = Flask(__name__)

app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

DATABASE = "../data/database.db"
init_database(DATABASE)

def importDico(longueurMotSecret) : # Récupère le dictionnaire des mots de longueur session["longueurMot"]
    chemin_dico = "../dictionary/ods/dico" + str(longueurMotSecret) + "lettres.txt"
    file = open(chemin_dico, "r")
    dicoAvantStrip = file.readlines()

    file.close()

    dico = []
    for mot in dicoAvantStrip :
        dico.append(mot.strip())
    return dico


def importDico_mot_simple(longueurMotSecret) : # Récupère le dictionnaire des mots de longueur session["longueurMot"]
    chemin_dico = "../dictionary/science_etonnante/mots_" + str(longueurMotSecret) + ".txt"
    file = open(chemin_dico, "r")
    dicoAvantStrip = file.readlines()

    file.close()

    dico = []
    for mot in dicoAvantStrip :
        dico.append(mot.strip())
    return dico

#print(DICO)

def choixMotSecret(dictionnaire) : # Choisit un mot au hasard dans le dictionnaire préalablement chargé
    motSecret = dictionnaire[rd.randint(0, len(dictionnaire))].lower()
    return motSecret

def hash_password(password) : # Hache un mot de passe
    return hashlib.sha256(password.encode()).hexdigest()

def genere_salt(longueur) : # Génère un salt (longueur = 10)
    characters = string.ascii_letters + string.digits + string.punctuation
    salt = ""
    for i in range(longueur) :
        salt += characters[rd.randrange(0, len(characters))]
    return salt
 
"""
#Wordle fonctionne sans la fonction test
#faire une fonction d'extraction des mots du fichier choisi dans une liste
def evaluationMotJoueur(motJoueur): # Prend en argument le mot choisi par le joueur                                
    bienPlace,malPlace=[],[]
    listeMotJoueur=list(motJoueur)
    listeMotSecret=list(motSecret)
    if len(listeMotJoueur)!=len(listeMotSecret):
        return "ErreurMotJoueurTailleDifferente"
    
    for i in range(len(listeMotSecret)):
        if listeMotSecret[i]==listeMotJoueur[i]:
            bienPlace.append(1)
            listeMotJoueur[i],listeMotSecret[i]=0,0
        else:
            bienPlace.append(0)
    
    for i in range(len(listeMotSecret)):
        if listeMotJoueur[i]!=0:
            if listeMotJoueur[i] in listeMotSecret:
                malPlace.append(1)
                listeMotSecret[listeMotSecret.index(listeMotJoueur[i])]=0
            else:
                malPlace.append(0)
        else:
            malPlace.append(0)
    return (bienPlace,malPlace)
"""
  
def getID(username) : # Récupère l'ID d'un joueur à partir de son pseudo | Retourne -1 si l'ID n'existe pas
    conn = sqlite3.connect(DATABASE)
    cur = conn.cursor()
    cur.execute("SELECT idUser FROM Utilisateur WHERE pseudo = ?", (username,))
    idUser = cur.fetchone()
    if idUser == None :
        return -1
    cur.close()
    conn.close()
    return idUser

def exp(nb_coups,t_mot,nb_essai): 
    return (nb_essai-nb_coups)*100+t_mot*200-nb_essai*20

def writeInWsolf(nombre): # Ecrit dans wsolf.txt la taille du mot de la partie en cours
    file = "../Solveur/wsolf.txt"
    pen = open(file, "w")
    pen.write(str(nombre))
    pen.close()

@app.before_first_request # Avant la 1ère requête (juste après avoir exécuté flask run), vide session et redirige vers /index
def before_first_request():
    print("@app.before_first_request")
    keys = session.keys()
    if "idUser" in keys:
        session.pop("idUser")
    if "username" in keys:
        session.pop("username")
    session["longueurMot"] = 7
    writeInWsolf(7)
    session["nombreEssais"] = 6
    session["modeJeu"] = "Classique"
    """
    print("before_first_request")
    for key, value in session.items():
        print(key, ' : ', value)
    """
    return redirect("/")

@app.route("/",methods=["GET", "POST"])     
def index():
    conn = sqlite3.connect(DATABASE)
    cur = conn.cursor()
    DICO = importDico(session["longueurMot"])
    motSecret = choixMotSecret(importDico_mot_simple(session["longueurMot"])) # Le mot à deviner
    #print("mot secret:",motSecret)

    if request.method == "POST" :
        session["longueurMot"] = request.form.get("tailleMots")
        writeInWsolf(session["longueurMot"])
        session["nombreEssais"] = request.form.get("nombreEssais")
        session["modeJeu"] = request.form.get("modeJeu")
        DICO = importDico(session["longueurMot"])
        motSecret = choixMotSecret(importDico_mot_simple(session["longueurMot"]))
    
    """
    print("/index")
    for key, value in session.items():
        print(key, ' : ', value)
    """

    return render_template("index.html", word = motSecret.upper() , dico = DICO, longueur = session["longueurMot"], nb_essais = session["nombreEssais"], modeJeu = session["modeJeu"])

@app.route("/register",methods=["POST","Get"])
def register():
    if request.method == "GET" :
        return render_template("register.html")
    
    elif request.method == "POST" :
        username = request.form.get("username")
        password = request.form.get("pass")
        confirmPass = request.form.get("confirmPass")
        if not (username and password and confirmPass) :
            return render_template("register.html", message = "Un ou plusieurs champs sont vides.")
        
        if password != confirmPass :
            return render_template("register.html", message = "Les mots de passe renseignés sont différents.")

        conn = sqlite3.connect(DATABASE)
        cur = conn.cursor()
        cur.execute("SELECT idUser FROM Utilisateur WHERE pseudo = ?", (username,))
        data = cur.fetchone()
        #print("register data :", data)
        if data == None : # Le pseudo est libre
            salt = genere_salt(10)
            cur.execute("insert into Utilisateur (pseudo, motDePasse, salt) values (?,?,?)", (username, hash_password(password + salt), salt))
            conn.commit()
        else :
            cur.close()
            conn.close()
            return render_template("register.html", message = "Ce pseudo est déjà pris par un autre joueur.")
        cur.close()
        conn.close()
        
        return redirect("/login")    

@app.route("/login",methods=["Get","Post"])
def login():
    if request.method == "GET" :
        return render_template("login.html")

    elif request.method == "POST" :
        username = request.form.get("username")
        password=request.form.get("pass")
        if not (username and password) :
            return render_template("register.html", message = "Un ou plusieurs champs sont vides.")
        
        conn = sqlite3.connect(DATABASE)
        cur = conn.cursor()
        cur.execute("SELECT idUser FROM Utilisateur WHERE pseudo = ?", (username,))
        data = cur.fetchone()
        #print("data :", data)
        if data == None : # Pseudo inexistant
            return render_template("login.html", message = "Les informations fournies sont incorrectes.")
        idUser = data[0]

        cur.execute("SELECT motDePasse, salt FROM Utilisateur WHERE idUser = ?", (idUser,))
        passwordData = cur.fetchall()[0]
        db_password, salt = passwordData[0], passwordData[1]
        if hash_password(password + salt) != db_password : # password : mdp tapé dans le form de connexion, db_password = mdp enregistré dans la BD
            return render_template("login.html", message = "Les informations fournies sont incorrectes.")
        else :
            session["idUser"] = idUser
            session["username"] = username

        cur.close()
        conn.close()
         
    return redirect("/")

@app.route("/logout")
def logout() :
    session.pop("idUser")
    session.pop("username")
    return redirect("/")

def listOfTuplesToList(listOfTuples) : # [(4,), (7,), (8,)] --> [4, 7, 8]
    L = []
    for i in range(len(listOfTuples)) :
        L.append(listOfTuples[i][0])
    return L

def canBeUnlocked(idUser) : # Retourne la liste des ids des achievements non débloqués par le joueur idUser
    conn = sqlite3.connect(DATABASE)
    cur = conn.cursor()
    #cur.execute("SELECT idSucces FROM ")
    cur.close()
    conn.close()

def chercheIndicePalierMaximumAtteint(nombre) : # Nécssaire pour achQuests() - renvoie l'indice du plus grand nombre dépassé dans palier.
    # 0-9 -> -1 | 10-19 -> 0 | 20-49 -> 1 | 50-99 -> 2 | 100-199 -> 3 | 200-499 -> 4 | 500+ -> 5
    paliers = [10, 20, 50, 100, 200, 500]
    indicePalierMax = -1
    for i in range(len(paliers)) :
        if nombre >= paliers[i] :
            indicePalierMax = i
    return indicePalierMax

def achQuests(idPartie) : # Toutes les requêtes SQL concernant les achievements
    #print("idPartie :", idPartie)
    achsAlreadyUnlocked = getAchsAlreadyUnlocked()
    paliers = [10, 20, 50, 100, 200, 500]

    conn = sqlite3.connect(DATABASE)
    cur = conn.cursor()
    #print("achQuests - begin")

    # Gagner x parties
    cur.execute("SELECT COUNT(*) FROM Partie WHERE nb_coups != -1 AND idUser = ?", (session["idUser"],))
    partiesGagnees = cur.fetchone()[0]
    indicePalierGagner = chercheIndicePalierMaximumAtteint(partiesGagnees)
    for i in range(0, indicePalierGagner + 1) :
        if indicePalierGagner >= 0 and achsAlreadyUnlocked[i] == 0 :
            #print("Gagné", i + 1)
            cur.execute("INSERT INTO Achievements VALUES (?, ?)", (idPartie, i + 1))
    #print(partiesGagnees, indicePalierGagner)

    # Jouer x parties
    cur.execute("SELECT COUNT(*) FROM Partie WHERE idUser = ?", (session["idUser"],))
    partiesJouees = cur.fetchone()[0]
    indicePalierJouer = chercheIndicePalierMaximumAtteint(partiesJouees)
    for i in range(0, indicePalierJouer + 1):
        if indicePalierJouer >= 0 and achsAlreadyUnlocked[i + 6] == 0 :
            #print("Joué", i + 7)
            cur.execute("INSERT INTO Achievements VALUES (?, ?)", (idPartie, i + 7))
    #print(partiesJouees, indicePalierJouer)

    # Gagner une partie en 3 essais ou moins
    cur.execute("SELECT COUNT(*) FROM Partie WHERE nb_coups <= 3 AND nb_coups != -1 AND idUser = ?", (session["idUser"],))
    troisCoupsOuMoins = cur.fetchone()[0]
    if achsAlreadyUnlocked[12] == 0 and troisCoupsOuMoins > 0 :
        cur.execute("INSERT INTO Achievements VALUES (?, ?)", (idPartie, 13))
    #print(troisCoupsOuMoins)

    # Jouer une partie avec chaque longueur de mot (de 4 à 12 lettres)
    cur.execute("SELECT DISTINCT longueur_mot FROM Partie WHERE idUser = ? ORDER BY longueur_mot ASC", (session["idUser"],))
    jouerToutesLesLongueurs = listOfTuplesToList(cur.fetchall())
    if achsAlreadyUnlocked[13] == 0 and jouerToutesLesLongueurs == range(4, 13) :
        cur.execute("INSERT INTO Achievements VALUES (?, ?)", (idPartie, 14))
    #print(jouerToutesLesLongueurs)

    # Gagner une partie avec chaque longueur de mot (de 4 à 12 lettres)
    cur.execute("SELECT DISTINCT longueur_mot FROM Partie WHERE nb_coups != -1 AND idUser = ? ORDER BY longueur_mot ASC", (session["idUser"],))
    gagnerToutesLesLongueurs = listOfTuplesToList(cur.fetchall())
    if achsAlreadyUnlocked[14] == 0 and gagnerToutesLesLongueurs == range(4, 13) :
        cur.execute("INSERT INTO Achievements VALUES (?, ?)", (idPartie, 15))
    #print(gagnerToutesLesLongueurs)

    # Gagner 5 parties consécutives, perdre 3 parties consécutives
    cur.execute("SELECT nb_coups FROM Partie WHERE idUser = ? ORDER BY idPartie DESC", (session["idUser"],))
    dataNbCoups = listOfTuplesToList(cur.fetchall())
    if achsAlreadyUnlocked[15] == 0 and cinqPositifsConsecutifs(dataNbCoups) :
        cur.execute("INSERT INTO Achievements VALUES (?, ?)", (idPartie, 16))
    if achsAlreadyUnlocked[16] == 0 and troisMoinsUnConsecutifs(dataNbCoups) :
        cur.execute("INSERT INTO Achievements VALUES (?, ?)", (idPartie, 17))
    #print(dataNbCoups)

    # Gagner une partie au dernier essai possible
    cur.execute("SELECT COUNT(*) FROM Partie WHERE idUser = ? AND nb_essais_max = nb_coups", (session["idUser"],))
    dernierEssaiPossible = cur.fetchone()[0]
    if achsAlreadyUnlocked[17] == 0 and dernierEssaiPossible > 0 :
        cur.execute("INSERT INTO Achievements VALUES (?, ?)", (idPartie, 18))
    #print(dernierEssaiPossible)

    #print("achQuests - end")

    conn.commit()
    cur.close()
    conn.close()

def cinqPositifsConsecutifs(liste) :
    c = 0
    for i in range(len(liste)) :
        if liste[i] >= 0 :
            c += 1
        else :
            c = 0
        if c == 5 :
            return True
    return False

def troisMoinsUnConsecutifs(liste) :
    c = 0
    for i in range(len(liste)) :
        if liste[i] == -1 :
            c += 1
        else :
            c = 0
        if c == 3 :
            return True
    return False

def getBestStreak(): # Retourne la plus grande série de victoires du joueur n°idUser
    conn = sqlite3.connect(DATABASE)
    cur = conn.cursor()
    cur.execute("select nb_coups from Partie where idUser="+str(session["idUser"]))
    serie_gagne=cur.fetchall()
    cur.close()
    conn.close
    
    M,k=0,0

    for i in serie_gagne:
        if i[0]!=-1:
            k+=1
        else:
            M=max(k,M)
            k=0
    M=max(k,M)
    return M

def stat(data):            #la liste data contient les mots à chercher, et les coups associés et renvoie les stat, ratio victoire/nb partie, moyenne des coups, ect...
    nbCoupsTotal=0 # La somme du nombre de coups effectués sur l'ensemble des parties d'un joueur
    nbPartiesPerdues=0 # Le nombre de parties perdues par un joueur
    for partie in data:
        if partie[0]!=-1:
            nbCoupsTotal+=partie[0]
        else:
            nbPartiesPerdues+=1
            #nbCoupsTotal += nbEssaisMax + 1 : non implémenté dans la BD
    #print(nbCoupsTotal, nbPartiesPerdues)
    nbPartiesJouees = len(data)
    nbPartiesGagnees = nbPartiesJouees - nbPartiesPerdues
    if nbPartiesJouees == 0 : # Pour ne pas diviser par 0
        moy, ratio = 0, 0
    else :
        moy = nbCoupsTotal / nbPartiesJouees
        ratio = nbPartiesGagnees / nbPartiesJouees
    return moy, nbPartiesJouees, ratio * 100    # nbEssaisMax n'est pas encore dans la BD --> la moyenne est fausse

def niveau(pts_exp):
    seuilsXP = [int(1.05 * x**(9/4)) + 150 for x in range(99)]
    #print(seuilsXP)
    level = 1
    while True :
        if level == 100 :
            missingXP = 0
            remp = 100
            break
        if pts_exp >= seuilsXP[level - 1] :
            pts_exp -= seuilsXP[level - 1]
            level += 1
        else :
            missingXP = seuilsXP[level - 1] - pts_exp
            remp = round((missingXP / seuilsXP[level]) * 100, 2)
            break
    #print("Niveau :", level, remp, missingXP)
    return (level, remp, missingXP)

    """
    if pts_exp < 14800:
        return 1
    else:
        r = pts_exp%moyen
        level_plus   = pts_exp - r
        k = level_plus/moyen
        level = level + k
        return int(level)
    """

    #assert niveau(1332000) == 91
    #assert niveau(1346800)== 92

    ''' nb essai max  =  6
        nb coups      =  4
        longeuer	  =  6
    pts xp  = (6-4)*100 + (6)*(200)  -  4(20)
    200 + 1200 - 80   =  1480       #  assez moyen comme points_xp     '''

@app.route("/profile",methods=["Get"]) # Partie, statistiques et succès
def profile():
    if not session.get("username") :
        return redirect("/")
    conn = sqlite3.connect(DATABASE)
    cur = conn.cursor()
    cur.execute("select nb_coups,pts_experience,nb_coups,longueur_mot from Partie where idUser=?",(session["idUser"],))
    data=cur.fetchall()
    cur.execute("SELECT MAX(idPartie) FROM PARTIE")
    idDernierePartie = cur.fetchone()[0]
    exp=0
    for i in data:
        exp+=i[1] # [1] --> pts_experience
    level, remp, missingXP = niveau(exp)   # ----->   niveau 
    """
    nb_lettre,result=[0],[] # nb_lettre : liste contenant toutes les différentes longueurs de mots déjà jouées par le joueur

    for i in data:
        if i[3] not in nb_lettre:
            nb_lettre.append(i[3])
    nb_lettre.sort()
    
    liste_result=[[0,data]]+[[i,[]] for i in nb_lettre[1:]] # liste_result : en général et pour chaque lettre, donne les stats associées

    for i in data:
        for j in range(len(liste_result)):
            if liste_result[j][0]==i[3]:
                liste_result[j][1].append(i)

    resultat=[[i[0],stat(i[1])] for i in liste_result]
    #print("resultat :", resultat)
    cur.close()
    conn.close()
    #print(achievements())

    grades=["novice","débutant","confirmé","sénior","maitre","expert"]
    pts=[0,500,1000,2000,5000,10000]


    #for i in range(len(pts)):
        #if exp>=pts[i]:
            #grade=grades[i]
            #if i!=len(grades):
                #grade2=grades[i+1]

    #pts_atteindre=pts[grades.index(grade2)]-exp     #nb de points pour le prochain grade
    #remp=exp/pts[grades.index(grade2)]*100
    #print(remp)
    """

    history = getHistorique()
    achQuests(idDernierePartie)
    stats = getStatistiques()
    #remp = round(( (14800 - (exp % 14800)) / 100), 1)
    
    #return render_template("profile.html",resultat=resultat,success=achievements(),exp=exp,grade=grade,grade2=grade2,pts_atteindre=pts_atteindre,remp=0, history = history)
    return render_template("profile.html", exp = exp, level = level, missingXP = missingXP, history = history, achDesc = getAchDesc(), achUnlocked = getAchsAlreadyUnlocked(), stats = stats, bestStreak = getBestStreak(), remp = remp)

@app.route("/recup/<userinfo>",methods=["POST"])
def recup(userinfo):
    userinfo = json.loads(userinfo)
    conn = sqlite3.connect(DATABASE)
    cur = conn.cursor()
    if session.get("username") :
        print("online")
        pts_exp = 0
        nbcoups = userinfo['essais']
        lmot = userinfo['longueur_mot']
        essaimax = userinfo['essais_max']
        modejeu = userinfo['modejeu']
        if nbcoups != -1:
            pts_exp = exp(nbcoups,lmot,essaimax)
                
        cur.execute("INSERT INTO Partie(idUser,motSecret,pts_experience,nb_essais_max,nb_coups,longueur_mot,modejeu) VALUES(?,?,?,?,?,?,?)",(session["idUser"],userinfo['motsecret'],pts_exp,userinfo['essais_max'],userinfo['essais'],userinfo['longueur_mot'],modejeu))
        conn.commit()
        
    return f"le nombre de lignes de user est {userinfo['essais']}"
       

@app.route("/preference")
def preference():
    tailleMots = [i for i in range(4, 13)]
    nombreEssais = [i for i in range(3, 13)]
    #mode = ["classique","infinie","1 lettre donnée","1 lettre bien placée","contre la montre"]
    return render_template("preference.html", tailleMots = tailleMots, nombreEssais = nombreEssais)
    
    
def getHistorique(): # Récupère l'ensemble des infos relatives aux parties du joueur n°idUser
    conn = sqlite3.connect(DATABASE)
    cur = conn.cursor()
    cur.execute("select * from Partie where idUser = ? ORDER BY idPartie DESC", (session["idUser"],))
    data=cur.fetchall()
    cur.close()
    conn.close()
    #print("Historique :", data)
    return data

def getAchDesc() : # Récupère les descriptions de tous les succès
    conn = sqlite3.connect(DATABASE)
    cur = conn.cursor()
    cur.execute("SELECT description FROM AchievementDesc ORDER BY idSucces ASC")
    data = cur.fetchall()
    cur.close()
    conn.close()
    #print("Descriptions des succès :", data)
    return data

def getAchsAlreadyUnlocked() : # Récupère l'ensemble des infos relatives aux succès du joueur n°idUser
    # Retourne un tableau de 0 et de 1, il y a des 1 aux indices des succès obtenus
    conn = sqlite3.connect(DATABASE)
    cur = conn.cursor()

    cur.execute("SELECT idSucces FROM Achievements JOIN Partie ON Partie.idPartie = Achievements.idPartie JOIN Utilisateur ON Utilisateur.idUser = Partie.idUser WHERE Utilisateur.idUser = ? ORDER BY idSucces ASC", (session["idUser"],))
    data = listOfTuplesToList(cur.fetchall())
    #print("Fetch des succès :", data)

    achs = [0] * len(getAchDesc())
    for i in range(len(data)) :
        achs[data[i] - 1] = 1 # Décalage dû au SQL
    cur.close()
    conn.close()
    #print("Succès obtenus :", achs)
    return achs

def getStatistiques() :
    conn = sqlite3.connect(DATABASE)
    cur = conn.cursor()

    ## Moyenne (pour les parties gagnantes)

    # Global
    cur.execute("SELECT AVG(nb_coups) FROM Partie WHERE idUser = ? AND nb_coups != -1", (session["idUser"],))
    data = cur.fetchone()[0]
    if data == None :
        moyenneCoupsGlobal = 0
    else :
        moyenneCoupsGlobal = round(data, 3)

    # Pour chaque lettre
    moyenneCoupsParLettre = []
    for i in range(4, 13) :
        cur.execute("SELECT AVG(nb_coups) FROM Partie WHERE idUser = ? AND nb_coups != -1 AND longueur_mot = ?", (session["idUser"], i))
        data = cur.fetchone()[0]
        #print(data)
        if data == None :
            moyenneCoupsParLettre.append(0)
        else :
            moyenneCoupsParLettre.append(round(data, 3))

    ## Nombre de parties jouées

    # Global
    cur.execute("SELECT COUNT(*) FROM Partie WHERE idUser = ?", (session["idUser"],))
    partiesJoueesGlobal = cur.fetchone()[0]

    # Pour chaque lettre
    partiesJoueesParLettre = []
    for i in range(4, 13) :
        cur.execute("SELECT COUNT(*) FROM Partie WHERE idUser = ? AND longueur_mot = ?", (session["idUser"], i))
        partiesJoueesParLettre.append(cur.fetchone()[0])

    ## Taux de victoires
    
    # Global
    cur.execute("SELECT COUNT(*) FROM Partie WHERE idUser = ? AND nb_coups != -1", (session["idUser"],))
    partiesGagnees = cur.fetchone()[0]
    if partiesJoueesGlobal == 0 :
        tauxVictoiresGlobal = 0
    else :
        tauxVictoiresGlobal = round(partiesGagnees / partiesJoueesGlobal * 100, 1)

    # Pour chaque lettre
    tauxVictoiresParLettre = []
    for i in range(4, 13) :
        cur.execute("SELECT COUNT(*) FROM Partie WHERE idUser = ? AND nb_coups != -1 AND longueur_mot = ?", (session["idUser"], i))
        partiesGagneesLongueurChoisie = cur.fetchone()[0]
        if partiesJoueesParLettre[i - 4] == 0 :
            tauxVictoiresParLettre.append(0)
        else :
            tauxVictoiresParLettre.append(round(partiesGagneesLongueurChoisie / partiesJoueesParLettre[i - 4] * 100, 1))

    cur.close()
    conn.close()
    stats = (moyenneCoupsGlobal, moyenneCoupsParLettre, partiesJoueesGlobal, partiesJoueesParLettre, tauxVictoiresGlobal, tauxVictoiresParLettre)
    #print(stats)
    return stats

