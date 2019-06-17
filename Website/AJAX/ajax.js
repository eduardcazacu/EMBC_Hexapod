var blockValues = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];  //15 blocks
//update loop
setInterval(timedRequests, 100);

function timedRequests() {
    //check if continuous scanning:
    var checkBox = document.getElementById("contScanCheckbox");

    if (checkBox.checked == true) {
        updateView();
    }
}

function requestScan() {
    var checkBox = document.getElementById("contScanCheckbox");
    if (checkBox.checked == false) {
        //trigger a new scan.
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "?cmd=1", true);  //1 for one scan
        xhttp.send();

        console.log("requesting one scan");

        //trigger a timed update
        setTimeout(updateView, 200);
    }
    else {
        console.log("continuous scan already eneabled");
    }
}
function enableContinuous() {
    //send request to server to start continuous scan
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "?cmd=2", true);  //2 for start autoscan
    xhttp.send();

    //console.log("requesting auto scan");

}
function disableContinuos() {
    //send request to server to disable the continuous scanning
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "?cmd=3", true);  //3 for stop autoscan
    xhttp.send();

    //console.log("requesting disable auto scan");
}
function updateView() {

    console.log("View Updating...");
    //request new data

    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            //data has been received:
            console.log(this.responseText);
            //parse the data
            //the format is 2 bytes per block. first byte is block id  (0 to 14) the second byte is the block value (0 to 255) with 0 the object is closest.
            parseData(this.responseText);


        }
    };
    xhttp.open("GET", "data.html", true);
    xhttp.send();

    //update the blocks to display that data
    updateBlocks();


}


function hexToOpacity(hexVal) {
    return hexVal / 255;
}

function updateBlocks() {
    var i;
    var idString = "";
    for (i = 0; i < 15; i++) {
        idString = "b" + i.toString();
        document.getElementById(idString).style.opacity = hexToOpacity(blockValues[i]);
        //console.log(idString);
    }

    //console.log("block style updated");
}

function simulatedDataRequest() {
    var data = "";
    data += String.fromCharCode(10);
    data += String.fromCharCode(10);
    data += String.fromCharCode(11);
    data += String.fromCharCode(50);
    data += String.fromCharCode(12);
    data += String.fromCharCode(180);
    data += String.fromCharCode(13);
    data += String.fromCharCode(255);
    data += String.fromCharCode(14);
    data += String.fromCharCode(180);
    data += String.fromCharCode(15);
    data += String.fromCharCode(0);
    parseData(data);
    // console.log("parsed block values are: ");
    //console.log(blockValues);

    //update bocks:
    updateBlocks();
}


function parseData(data) {
    var i;
    //console.log("parsed dec values are:\n");
    for (i = 0; i < data.length; i = i + 2) {
        blockValues[data.charCodeAt(i) - 1] = data.charCodeAt(i + 1);
    }
}

function move(cmd) {
    //based on the cmd (button press/release)
    //options: fwd pressed, fwd released, left pressed, left released, etc
    //send the correct request to the server with an agreed uppon protocol.
    var req = 0;
    var reqString = "";
    if (cmd == "fwd pressed") {
        console.log("Moving forward");
        req = 1;
    }
    else if (cmd == "fwd released") {
        console.log("Stop moving forward");
        req = 2;
    }
    else if (cmd == "bwd pressed") {
        console.log("Moving backwords");
        req = 3;
    }
    else if (cmd == "bwd released") {
        console.log("Stop moving backward");
        req = 4;
    }
    else if (cmd == "left pressed") {
        console.log("turning left");
        req = 5;
    }
    else if (cmd == "left released") {
        console.log("Stop turning left");
        req = 6;
    }
    else if (cmd == "right pressed") {
        console.log("turning right");
        req = 7;
    }
    else if (cmd == "right released") {
        console.log("Stop turning right");
        req = 8;
    }
    else {
        console.log("invalid command");
        req = 0;
    }

    //send the request:
    reqString = "?move="+req.toString();
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", reqString, true);  
    xhttp.send();
}

