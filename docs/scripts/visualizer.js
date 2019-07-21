var octree = {root: "0"};

window.onload = function() {
    var canvas = document.getElementById("canvas");

    canvas.width = window.innerWidth / 2;
    canvas.height = window.innerHeight;

    var context = canvas.getContext('2d');

    var scaleX = 1;
    var scaleY = 1;
    var scaleZ = 1;
    var offsetX = canvas.width / 2;
    var offsetY = canvas.height / 2;

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

    var node_stack = [];
    var x_stack = [];
    var y_stack = [];
    var z_stack = [];
    var w_stack = [];
    var h_stack = [];
    var d_stack = [];

    function fillExplorer(node, x, y, z, w, h, d) {
        console.log(node, x, y, z, w, h, d, node_stack, x_stack, y_stack, z_stack, w_stack, h_stack, d_stack);
        var current_node = document.getElementById("current-node");
        current_node.innerText = node;
        var parent_td = document.getElementById("parent-node");
        if (node_stack.length === 0)
            parent_td.innerText = "(node is root)";
        else
            parent_td.innerText = node_stack[node_stack.length - 1];

        parent_td.onclick = function() {

            //console.log("...---:...");
            //console.log(node, x, y, z, w, h, d, node_stack, x_stack, y_stack, z_stack, w_stack, h_stack, d_stack);
            node_stack.pop();
            x_stack.pop();
            y_stack.pop();
            z_stack.pop();
            w_stack.pop();
            h_stack.pop();
            d_stack.pop();
            node_stack.pop();
            x_stack.pop();
            y_stack.pop();
            z_stack.pop();
            w_stack.pop();
            h_stack.pop();
            d_stack.pop();

            //console.log(node, x, y, z, w, h, d, node_stack, x_stack, y_stack, z_stack, w_stack, h_stack, d_stack);
            fillExplorer(
            node_stack[node_stack.length - 1],
            x_stack[x_stack.length - 1],
            y_stack[y_stack.length - 1],
            z_stack[z_stack.length - 1],
            w_stack[w_stack.length - 1],
            h_stack[h_stack.length - 1],
            d_stack[d_stack.length - 1]
        ); };

        var position_td = document.getElementById("position");
        position_td.innerText = x + ", " + y + ", " + z;

        var dimensions_td = document.getElementById("dimensions");
        dimensions_td.innerText = w + ", " + h + ", " + d;

        window.onresize = function() {
            scaleX = 1;
            scaleY = 1;
            scaleZ = 1;
            canvas.width = window.innerWidth / 2;
            canvas.height = window.innerHeight;
            offsetX = canvas.width / 2;
            offsetY = canvas.height / 2;
            var minDim = Math.min(canvas.width, canvas.height);
            context.clearRect(0, 0, canvas.width, canvas.height);
            printOctree(0, 0, 0, minDim / 3.5, minDim / 3.5, minDim / 3.5, octree.root);
            fillExplorer(node, x, y, z, w, h, d);
        };

        node_stack.push(node);
        x_stack.push(x);
        y_stack.push(y);
        z_stack.push(z);
        w_stack.push(w);
        h_stack.push(h);
        d_stack.push(d);

        for (var i = 0; i < 8; i++) {
            var child_node = document.getElementById("child" + i);
            if (octree["nodes"] != undefined && octree["nodes"][node] != undefined && octree["nodes"][node]["children"] != undefined) {
                const child_key = octree["nodes"][node]["children"][i];
                const chw = w * 0.5;
                const chh = h * 0.5;
                const chd = d * 0.5;
                const cx = x + (2 * (i % 2) - 1) * chw;
                const cy = y + (2 * (Math.floor(i / 2) % 2) - 1) * chh;
                const cz = z + (2 * Math.floor(i / 4) - 1) * chd;
                child_node.innerText = child_key;
                child_node.onclick = function() { fillExplorer(
                    child_key,
                    cx,
                    cy,
                    cz,
                    chw,
                    chh,
                    chd
                ); };
            }
        }

        var minDim = Math.min(canvas.width, canvas.height);
        context.clearRect(0, 0, canvas.width, canvas.height);
        printOctree(0, 0, 0, minDim / 3.5, minDim / 3.5, minDim / 3.5, node);
    }

    document.getElementById("octree-file")
        .addEventListener('change', readFile, false);

    function readFile(evt) {
        var file = evt.target.files[0];
        if (file) {
            var reader = new FileReader();
            reader.readAsText(file, "UTF-8");
            reader.onload = function (evt) {
                octree = JSON.parse(evt.target.result);
                var minDim = Math.min(canvas.width, canvas.height);
                printOctree(0, 0, 0, minDim / 3.5, minDim / 3.5, minDim / 3.5, octree.root);
                fillExplorer(octree.root, 0, 0, 0, 64, 64, 64);
            };
        }
    }

};
