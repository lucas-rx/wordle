const someAlertsBox = document.getElementById("some-alerts-box");
const notInDicoAlert = document.getElementById("not-in-dico-alert");
const defeatAlert = document.getElementById("defeat-alert");
const notEnoughLetterAlert = document.getElementById("not-enough-letter-alert");
const victoryAlert = document.getElementById("victory-alert");

const main = document.getElementById("main");
const rej = document.querySelector(".rejouer");

document.addEventListener("DOMContentLoaded", () => {
    createcarre();
    let words = [[]];
    let currentword = "";
    let ind_current_word = 1;
    let enterfinjeu = "";
    let modejeu = document.querySelectorAll("p")[4].getAttribute("class")

    let longueur = document.querySelectorAll("p")[2].getAttribute("class");
    let nb_essais = document.querySelectorAll("p")[3].getAttribute("class");
    let longueurf = parseInt(longueur, 10);
    let nb_essaisf = parseInt(nb_essais, 10);




    let w = document.querySelector("p").getAttribute("class");
    let dico = document.querySelectorAll("p")[1].getAttribute("class");
    let dicof = dico.substring(1, dico.length - 1).split(",");
    let ldico = [];
    //Récupération du dictionnaire
    for (let j = 0; j < dicof.length; j++) {
        if (dicof[j][0] == "'") {
            let m = dicof[j].substring(1, dicof[j].length - 1);
            ldico.push(m);
        }
        else {
            let m = dicof[j].substring(2, dicof[j].length - 1);
            ldico.push(m);

        }
    }
    let secret_word = w.toUpperCase();
    let copy_secret_word = secret_word;
    let alphabet = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"];
    //params longueur 20 
    function createcarre() {
        let longueur = document.querySelectorAll("p")[2].getAttribute("class");
        let nb_essais = document.querySelectorAll("p")[3].getAttribute("class");
        let longueurf = parseInt(longueur, 10);
        let nb_essaisf = parseInt(nb_essais, 10);

        const noeud = document.querySelector(".board");
        let k = 1;
        for (let i = 1; i < nb_essaisf + 1; i++) {
            const division = document.createElement("div");
            division.classList.add("axe");
            for (let j = 1; j < longueurf + 1; j++) {
                const newnch = document.createElement("div");
                newnch.classList.add("carre");
                newnch.setAttribute("id", k + j - 1);
                division.appendChild(newnch);

            }
            noeud.appendChild(division);
            k = k + longueurf;

        }
    }

    function getmotcourant() {
        return words[words.length - 1];

    }
    //mettre chaque lettre tapé dans les divs          // Modifs sur ces 3 lignes : si elles te gênent vire-les -Lucas
    window.addEventListener("keydown", (e) => { // document.addEventListener => window.addEventListener
        let mot = getmotcourant();
        let secret_word = w.toUpperCase();
        if ((alphabet.includes(e.key.toUpperCase())) && (secret_word.localeCompare(mot.join("")) != 0) && (words.length < nb_essaisf + 1)) { // Ajout de .toUpperCase()
            let letter = e.key.toUpperCase(); // Ajout de .toUpperCase()
            updateletter(letter);

        }

        else if ((e.key.localeCompare("Backspace") == 0)) {
            let secret_word = w.toUpperCase();
            let ind = ind_current_id();
            let word = getmotcourant();
            //console.log(word);
            //console.log(enterfinjeu);
            if (word.length > 0) {
                if (enterfinjeu == "enter") {
                    //fin

                }

                else {
                    let eff = document.getElementById(String(ind));

                    eff.innerText = "";

                    word.pop();
                }
            }

        }

        else if (e.key.localeCompare("Enter") == 0) {
            let word = [].concat(getmotcourant());
            let copy_word = [].concat(word);
            //console.log(copy_word);
            if (word.length == secret_word.length) {
                if (!ldico.includes(word.join(''))) {//si le mot n'est pas dans le dico

                    someAlertsBox.style.visibility = "visible";
                    notInDicoAlert.style.display = "inline";
                    setTimeout(hideNotInDicoAlert, 3000);
                    //window.alert("not word in list");
                }
                else {
                    let ind = ind_current_id();//id du carre courant
                    let secret_word = w.toUpperCase();
                    //colorier les carres
                    for (let j = 0; j < word.length; j++) {

                        document.getElementById(String(ind - word.length + j + 1)).style.backgroundColor = "#9B9894"; // Gris

                    }

                    for (let j = 0; j < word.length; j++) {
                        //console.log(word[j]);
                        if (word[j] == secret_word[j]) {
                            document.getElementById(String(ind - word.length + j + 1)).style.backgroundColor = "#2EDA3F"; // Vert
                            secret_word = secret_word.replace(word[j], "5");
                            word[j] = "5";
                            //console.log(secret_word);
                        }
                    }

                    for (let j = 0; j < word.length; j++) {
                        //console.log(word[j]);
                        if ((word[j] != "5") && (secret_word.includes(word[j])) && (secret_word[secret_word.indexOf(word[j])] != "5")) {
                            document.getElementById(String(ind - word.length + j + 1)).style.backgroundColor = "#E49339"; // Jaune
                            secret_word = secret_word.replace(word[j], "5");
                            //console.log(secret_word);
                        }
                    }

                    // fin du jeu ici
                    //Ajout bouton rejouer

                    if (((words.length == nb_essaisf)) || (copy_secret_word.localeCompare(copy_word.join("")) == 0)) {
                        enterfinjeu = "enter";
                        //RECUP les données

                        if (rej.textContent == "") {
                            rej.textContent = "Rejouer";
                            let N = words.length;

                            if (copy_secret_word.localeCompare(copy_word.join("")) != 0) {
                                N = -1;
                            }

                            let userinfo = {
                                'essais': N,
                                'longueur_mot': longueurf,
                                'motsecret': copy_secret_word,
                                'essais_max': nb_essaisf,
                                'modejeu': modejeu,
                            }

                            someAlertsBox.style.visibility = "visible";
                            if (userinfo["essais"] == -1) { // Défaite
                                defeatAlert.style.display = "inline";
                            } else { // Victoire
                                victoryAlert.style.display = "inline";
                            }

                            const xhml = new XMLHttpRequest();
                            url = "/recup/";
                            urlf = url.concat(JSON.stringify(userinfo));

                            xhml.open("POST", urlf);
                            xhml.onload = () => {
                                console.log(xhml.responseText);
                            }
                            xhml.send();

                            //prise en compte du mode de jeu
                            if ((modejeu != "Classique") && (copy_secret_word.localeCompare(copy_word.join("")) != 0)) {
                                //effacer et continuer
                                setTimeout("window.location.replace('http://127.0.0.1:5000/')", 2000);


                            }
                            else {

                                rej.style.visibility = "visible"; // On rend le bouton visible
                                main.style.position = "relative";
                                main.style.top = "70px";
                                /*const newe = document.createElement("a");
                                newe.setAttribute("href", "/");
                                newe.innerText = "Rejouer";
                                rej.appendChild(newe);*/


                            }

                        }
                    }

                    else if ((words.length < nb_essaisf) && (copy_secret_word.localeCompare(copy_word.join("")) != 0)) {
                        words.push([]);
                        if (alphabet.includes(e.key)) {
                            let letter = e.key;
                            updateletter(letter);
                        }
                    }


                }
            }
            else if ((copy_word.length < longueurf) && (words.length < nb_essaisf + 1)) {
                someAlertsBox.style.visibility = "visible";
                notEnoughLetterAlert.style.display = "inline";
                setTimeout(hideNotEnoughLetterAlert, 3000);
            }
        }


    });

    function ind_current_id() {
        let ind = 0;
        for (let i = 0; i < words.length; i++) {
            ind += words[i].length;
        }
        return ind;
    }


    function updateletter(lettre) {
        let word = getmotcourant();
        if (lettre && word.length < longueurf) {
            ind_current_word = ind_current_id() + 1;

            const newEl = document.getElementById(String(ind_current_word));
            if (newEl != null) {
                newEl.innerText = lettre;

            }

            word.push(lettre);

        }

    }

});

function hideNotInDicoAlert() { // Cache l'alerte informant le joueur de l'absence du mot saisi dans le dictionnaire du jeu
    someAlertsBox.style.visibility = "hidden";
    notInDicoAlert.style.display = "none";
}

function hideNotEnoughLetterAlert() {
    someAlertsBox.style.visibility = "hidden";
    notEnoughLetterAlert.style.display = "none";
}

//test fonction evaluation par affichage de la sortie
let smot1 = ["C", "O", "N", "S", "T", "I", "T", "U", "T", "I", "O", "N"];
let cmot1 = ["C", "O", "N", "S", "T", "I", "T", "U", "T", "I", "O", "N"];

let smot2 = ["T", "R", "A", "Q", "U", "E", "R"];
let cmot2 = ["P", "R", "I", "E", "R", "E", "S"];

let smot4 = ["M", "O", "T"];
let cmot4 = ["P", "A", "S"];

let smot3 = ["P", "E", "A", "U"];
let cmot3 = ["P", "E", "A", "U"];
function eval(smot, cmot) {

    //let ti1 = performance.now();

    for (let j = 0; j < cmot.length; j++) {
        if (cmot[j] == smot[j]) {
            // Vert
            //console.log(res);
            smot[smot.indexOf(cmot[j])] = "V";
            cmot[j] = "V";
            //console.log(smot);
            //console.log(res);
        }
    }

    for (let j = 0; j < cmot.length; j++) {
        //console.log(mot[j]);
        if ((cmot[j] != "V") && (smot.includes(cmot[j])) && (smot[smot.indexOf(cmot[j])] != "V")) {
            // Jaune
            smot[smot.indexOf(cmot[j])] = "J";
            cmot[j] = "J";
            //console.log(secret_word);
        }
    }

    for (let j = 0; j < cmot.length; j++) {
        if ((cmot[j] != "J") && (cmot[j] != "V")) {
            cmot[j] = "G";
        }
    }

    //let tf1 = performance.now();
    //console.log(tf1 - ti1);
    return cmot;

}
//console.time("#code4");
let mot4 = eval(smot4, cmot4);
//console.timeEnd("#code4");



//console.time("#code2");
let mot2 = eval(smot3, cmot3);
//console.timeEnd("#code2");

//console.time("#code1");
let ti1 = performance.now();
let mot1 = eval(smot2, cmot2);
let tf1 = performance.now();
console.log(tf1 - ti1);
//console.timeEnd("#code1");
//console.log(tf1 - ti1);

//console.time("#code3");
let ti2 = performance.now();
let mot3 = eval(smot1, cmot1);
let tf2 = performance.now();
console.log(tf2 - ti2);

//console.timeEnd("#code3");


//console.log(tf2 - ti2);



//console.log(tf3 - ti3);
