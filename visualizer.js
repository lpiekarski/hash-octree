var canvas = document.getElementById("canvas");

canvas.width = window.innerWidth;
canvas.height = window.innerHeight;

var context = canvas.getContext('2d');

var scaleX = 1;
var scaleY = 1;
var scaleZ = 1;
var offsetX = window.innerWidth / 2;
var offsetY = window.innerHeight / 2;

window.onresize = function() {
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;
    offsetX = window.innerWidth / 2;
    offsetY = window.innerHeight / 2;
};

var dirZ = [-0.54, 0.467];

function XYZToXY(xyz) {
    return [xyz[0] * scaleX + xyz[2] * dirZ[0] * scaleZ,
            xyz[1] * scaleY + xyz[2] * dirZ[1] * scaleZ];
}

function printBox(x, y, z, hw, hh, hd) {
    var face1 = [
        [x - hw, y - hh, z - hd],
        [x + hw, y - hh, z - hd],
        [x - hw, y + hh, z - hd],
        [x + hw, y + hh, z - hd]];
    var face2 = [
        [x - hw, y - hh, z + hd],
        [x + hw, y - hh, z + hd],
        [x - hw, y + hh, z + hd],
        [x + hw, y + hh, z + hd]];
    var f1RectX = XYZToXY(face1[0])[0];
    var f1RectY = XYZToXY(face1[0])[1];
    var f1RectW = XYZToXY([hw * 2, hh * 2, 0])[0];
    var f1RectH = XYZToXY([hw * 2, hh * 2, 0])[1];
    var f2RectX = XYZToXY(face2[0])[0];
    var f2RectY = XYZToXY(face2[0])[1];
    var f2RectW = XYZToXY([hw * 2, hh * 2, 0])[0];
    var f2RectH = XYZToXY([hw * 2, hh * 2, 0])[1];
    context.strokeRect(offsetX + f1RectX, offsetY + f1RectY, f1RectW, f1RectH);
    context.strokeRect(offsetX + f2RectX, offsetY + f2RectY, f2RectW, f2RectH);
    context.beginPath();
    for (var id in face1) {
        var xyz1 = face1[id];
        var xyz2 = face2[id];
        var xy1 = XYZToXY(xyz1);
        var xy2 = XYZToXY(xyz2);
        context.moveTo(offsetX + xy1[0], offsetY + xy1[1]);
        context.lineTo(offsetX + xy2[0], offsetY + xy2[1]);
    }
    context.stroke();
}

function printOctree(x, y, z, hw, hh, hd, key) {
    if (key === "0")
        return;
    console.log(key);
    context.strokeStyle = "rgb(" + (octree.nodes[key].data % 256) +
        ", " + (Math.floor(octree.nodes[key].data / 256) % 256) +
        ", " + (Math.floor(octree.nodes[key].data / (256 * 256)) % 256) + ")";
    printBox(x, y, z, hw, hh, hd);
    for (var i = 0; i < 8; i++) {
        var chw = hw * 0.5;
        var chh = hh * 0.5;
        var chd = hd * 0.5;
        var cx = x + (2 * (i % 2) - 1) * chw;
        var cy = y + (2 * (Math.floor(i / 2) % 2) - 1) * chh;
        var cz = z + (2 * Math.floor(i / 4) - 1) * chd;
        var ckey = octree.nodes[key].children[i];
        printOctree(cx, cy, cz, chw, chh, chd, ckey);
    }
}


var octree = {root: "0"};

document.getElementById("octree-file")
    .addEventListener('change', readFile, false);
function readFile(evt) {
    var file = evt.target.files[0];
    if (file) {
        console.log("file uploaded");
        var reader = new FileReader();
        reader.readAsText(file, "UTF-8");
        reader.onload = function (evt) {
            octree = JSON.parse(evt.target.result);
            var minDim = Math.min(window.innerWidth, window.innerHeight);
            printOctree(0, 0, 0, minDim / 3, minDim / 3, minDim / 3, octree.root);
        };
    }
}