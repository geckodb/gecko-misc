var width = 1050,
    height = 580;
var tempArray=new Array();
var dataArray = new Array();
var linksArray = new Array();
var authorAlreadyAdded=new Map();
var paperAlreadyAdded=new Map();
var paperExpanded= new Set();
var nodeExpandedforRefernce = new Set();
var nodeAdded= new Set();
var fullscreen=true;

var svg = d3.select("#paperGraphArea")
    .append("svg")
    .attr("width",width)
    .attr("height",height);

var menuItems = ["Authorship", "Citedby", "References"];
var authorMenuItems=["Co-word", "Bibliographic"];

var force = d3.layout.force()
    .distance(100)
    .linkDistance([150])
    .charge(-500)
    .size([width, height])
    .gravity(0.1)
    .alpha(0);

d3.json("../JSON/Completedata.json", function (error, json) {
    if (error) throw error;

    dataArray = json.hits;
    console.log(dataArray)
    d3.json("../JSON/temp.json",tempArray);
});

function poplateClickedNode(nodeId) {
    for(let i=0;i<dataArray.length;i++){
        if(nodeId===dataArray[i].PaperId && (!nodeAdded.has(nodeId))){
            tempArray.push(dataArray[i]);
            nodeAdded.add(nodeId);
            d3.selectAll("svg > *").remove();
            createGraph(tempArray,linksArray,false);
            break;
        }
    }
}

function createGraph(nodes, links, drawnodesOnly) {
    var label=new Array();

//To form arrowhead
    svg.append("defs").selectAll("marker")
        .data(["arrowhead", "licensing", "resolved"])
        .enter().append("marker")
        .attr("id", function (d) {
            return d;
        })
        .attr("viewBox", "0 -5 10 10")
        .attr("refX", 24)
        .attr("refY", 0)
        .attr("markerWidth", 25)
        .attr("markerHeight", 15)
        .attr("orient", "auto")
        .append("path")
        .attr('d', 'M 0,-5 L 10 ,0 L 0,5')
        .attr('fill', '#ccc')
        .attr('stroke', '#ccc');


    force.nodes(nodes)
        .links(links)
        .start();

    var link = svg.selectAll("link")
        .data(links)
        .enter().append("line")
        .attr("class", "link")
        .attr("marker-end", "url(#arrowhead)")
        .attr('fill', '#ccc')
        .attr('stroke', '#ccc');

    var node_drag = d3.behavior.drag()
        .on("dragstart", drag_start)
        .on("drag", drag_move)
        .on("dragend", drag_end);


    function drag_start(d, i) {
        force.stop() // stops the force auto positioning before you start dragging
    }

    function drag_move(d, i) {
        d.px += d3.event.dx;
        d.py += d3.event.dy;
        d.x += d3.event.dx;
        d.y += d3.event.dy;
        tick(); // this is the key to make it work together with updating both px,py,x,y on d !
    }

    function drag_end(d, i) {
        d.fixed = true; // set the node to fixed, so the force doesn't include the node in its auto positioning
        tick();
        force.resume();
    }

    var edgepaths = svg.selectAll(".edgepath")
        .data(links)
        .enter()
        .append('path')
        .attr({'d': function(d) { return 'M '+d.source.x+' '+d.source.y+' L '+ d.target.x +' '+d.target.y},
            'class':'edgepath',
            'fill-opacity':0,
            'stroke-opacity':0,
            'id':function(d,i) { return 'edgepath'+i;}})
        .style("pointer-events", "none");

    var edgelabels = svg.selectAll(".edgelabel")
        .data(links)
        .enter()
        .append('text')
        .style("pointer-events", "none")
        .attr({'class':'edgelabel',
            'id':function(d,i){return 'edgelabel'+i;},
            'dx':80,
            'dy':0,
            'font-size':10,
            'fill':'#aaa'});

    edgelabels.append("textPath")
        .attr('xlink:href',function(d,i) {return '#edgepath'+i})
        .style("pointer-events", "none")
        .text(function(d,i){
            if((d.target.type==="paper")||(d.target.type==="paper_new")){
                return "cited by"
            }else if(d.target.type==="author"){
                return "author"
            }else if(d.target.type==="reference"){
                return "refers to"
            }

        });


    var node = svg.selectAll("node")
        .data(nodes)
        .enter().append("g")
        .attr("class", "node")
        .call(node_drag);

    node.append("image")
        .attr("xlink:href", function (d) {
            if((d.type==="paper") || (d.type==="reference")||(d.type==="paper_new")) {
                return "http://icons.iconarchive.com/icons/pelfusion/long-shadow-media/512/Document-icon.png"
            }
            else if (d.type==="author"){
                return "http://www.clker.com/cliparts/3/V/U/m/W/U/admin-button-icon-md.png"
            }
        })
        .attr("x", -8)
        .attr("y", -8)
        .attr("width",function (d) {
            if(d.PaperId==="456"){
                return 30;
            }else{
                return 30;
            }
        })
        .attr("height", function (d) {
            if(d.PaperId==="456"){
                return 30;
            }else{
                return 30;
            }
        })
        .attr("alt","abc")

        //to populate context menu
        .on('contextmenu', function (d, i) {
            // create the div element that will hold the context menu
            d3.selectAll('.context-menu').data([1])
                .enter()
                .append('div')
                .attr('class', 'context-menu');
            // close menu
            d3.select('body').on('click.context-menu', function () {
                d3.select('.context-menu').style('display', 'none');
            });
            // this gets executed when a context menu event occurs
            d3.selectAll('.context-menu')
                .html('')
                .append('ul')
                .selectAll('li')
                .data(function () {
                    if((d.type==="paper")||(d.type==="paper_new"))
                        return menuItems;
                    else
                        return authorMenuItems;
                }).enter()
                .append('li')
                .on('click', function (d) {
                    console.log("context menu-" +d);
                    if((d==="Authorship")){
                        showAuthors(paperId);
                        paperExpanded.add(paperId);
                    }else if(d==="Citedby"){
                        showCitations(paperId);
                    }else if(d==="References"){
                        showReferences(paperId);
                        nodeExpandedforRefernce.add(paperId);
                    }
                })
                .text(function (d) { console.log(d); return d;});


            d3.select('.context-menu').style('display', 'none');
            // show the context menu
            d3.select('.context-menu')
                .style('left',d3.event.pageX - 2 + 'px')
                .style('top', d3.event.pageY - 2 + 'px')
                .style('display', 'block');
            d3.event.preventDefault();
            var paperId = d.PaperId;
        })
        //To display tooltip
        .on("mouseover", function (d) {
            if((d.type==="paper")||(d.type==="paper_new")){
                div.transition()
                    .duration(200)
                    .style("opacity", .9);
                div.html(d.title + "<br/>")
                    .style("left", (d3.event.pageX) + "px")
                    .style("top", (d3.event.pageY - 30) + "px");
            }
            else if(d.type==="author"){
                div.transition()
                    .duration(200)
                    .style("opacity", .9);
                div.html(d.authors + "<br/>")
                    .style("left", (d3.event.pageX) + "px")
                    .style("top", (d3.event.pageY - 30) + "px");
            }
        })
        //to disbale tooltip
        .on("mouseout", function (d) {
            div.transition()
                .duration(500)
                .style("opacity", 0);
        });

    node.append("title")
        .attr("dx", 20)
        .attr("dy", ".35em");


    node.append("text")
        .attr("dx", 20)
        .attr("dy", ".35em")
        .text(function (d) {
            if((d.type==="paper")||(d.type==="paper_new")){
                return d.properties.title;
            }
            else if(d.type==="author"){
                return d.authors;
            }
            else if(d.type==="reference"){
                return d.properties.title;
            }
        });

    //	Define the div for the tooltip
    var div = d3.select("body").append("div")
        .attr("class", "tooltip")
        .style("opacity", 0);
    if(!drawnodesOnly)
        force.on("tick",tick );
    else
        force.on("tick",node_tick );

    function node_tick() {
        node.attr("transform", function (d) { return "translate(" + d.x + "," + d.y + ")"; });
    }

    function tick() {
        if (!drawnodesOnly) {
            link.attr("x1", function (d) {
                return d.source.x;
            })
                .attr("y1", function (d) {
                    return d.source.y;
                })
                .attr("x2", function (d) {
                    return d.target.x;
                })
                .attr("y2", function (d) {
                    return d.target.y;
                });


            node.attr("transform", function (d) {
                return "translate(" + d.x + "," + d.y + ")";
            });

            edgepaths.attr('d', function (d) {
                return 'M ' + d.source.x + ' ' + d.source.y + ' L ' + d.target.x + ' ' + d.target.y;
            });

            edgelabels.attr('transform', function (d, i) {
                if (d.target.x < d.source.x) {
                    bbox = this.getBBox();
                    rx = bbox.x + bbox.width / 2;
                    ry = bbox.y + bbox.height / 2;
                    return 'rotate(180 ' + rx + ' ' + ry + ')';
                }
                else {
                    return 'rotate(0)';
                }
            });
        }
    }
}

function resize() {
    if(fullscreen){
        $("#graphArea").removeClass("startSize").addClass("newSize");

        fullscreen=false;
    }
    else{
        $("#graphArea").removeClass("newSize").addClass("startSize");

        fullscreen=true;
    }
}

function createAuthorNode(paperId, authorName) {
    this.authorId = paperId;
    this.authors = authorName;
    this.type="author";
}

function createPaperNode(paperId, properties) {
    this.PaperId = paperId;
    this.properties = properties;
    this.type="paper_new";
}

function createLinks(source, target) {
    this.source = source;
    this.target = target;
}

function createRefernceNode(paperId, properties) {
    this.PaperId = paperId;
    this.properties = properties;
    this.type="reference";
}

function showAuthors(idToEXpand) {

    var intial_length = tempArray.length;
    for (var i = 0; i < intial_length; i++) {
        if (tempArray[i].type === "paper") {
            if ((tempArray[i].PaperId === idToEXpand) && (tempArray[i].properties.authors !== undefined)) {
                for (var j = 0; j < tempArray[i].properties.authors.length; j++) {
                    if (!authorAlreadyAdded.has(tempArray[i].properties.authors[j].name)) {
                        var newNode = new createAuthorNode(tempArray[i].properties.PaperId, tempArray[i].properties.authors[j].name)
                        var index = tempArray.push(newNode);
                        authorAlreadyAdded.set(tempArray[i].properties.authors[j].name, index - 1);
                        var newLink = new createLinks(i, tempArray.length - 1);
                        linksArray.push(newLink);
                    }else{
                        var newLink = new createLinks(i, authorAlreadyAdded.get(tempArray[i].properties.authors[j].name));
                        linksArray.push(newLink);
                    }
                }
            }
        }
    }

    for (var i = 0; i < intial_length; i++) {
        if (tempArray[i].type === "paper") {
            if ((tempArray[i].PaperId !== idToEXpand) && (!paperExpanded.has(tempArray[i].PaperId)) && (tempArray[i].properties !== undefined)) {
                for (var j = 0; j < tempArray[i].properties.authors.length; j++) {
                    if (authorAlreadyAdded.has(tempArray[i].properties.authors[j].name)) {
                        var newLink = new createLinks(i, authorAlreadyAdded.get(tempArray[i].properties.authors[j].name));
                        linksArray.push(newLink);
                    }
                }
            }
        }
    }

    JSON.stringify(linksArray);
    JSON.stringify(tempArray);
    d3.selectAll("svg > *").remove();
    var mydata=new Set(tempArray);
    for(let item of mydata) console.log(item);
    createGraph(tempArray, linksArray,false);
    d3.select('.context-menu').style('display', 'none');
}

function showCitations(idToEXpand) {
    var intial_length = tempArray.length;
    for (var i = 0; i < intial_length; i++) {
        if(tempArray[i].type==="paper"){
            if((tempArray[i].PaperId===idToEXpand)&&(tempArray[i].properties.inE!==undefined)) {
                for (j = 0; j < tempArray[i].properties.inE[0].vertexProperties.length; j++) {
                    if (!paperAlreadyAdded.has(tempArray[i].properties.inE[0].vertexProperties[j].PaperId)) {
                        var newNode = new createPaperNode(tempArray[i].properties.inE[0].vertexProperties[j].PaperId, tempArray[i].properties.inE[0].vertexProperties[j].properties);
                        var index = tempArray.push(newNode);
                        var newLink = new createLinks(i, tempArray.length - 1);
                        paperAlreadyAdded.set(tempArray[i].properties.inE[0].vertexProperties[j].PaperId, index - 1);
                        linksArray.push(newLink);
                    }
                }
            }

        }

    }

    //creates links to existing nodes if new nodes have any relation with it
    for (var i = 0; i < intial_length; i++) {
        if (tempArray[i].type === "paper") {
            if ((tempArray[i].PaperId !== idToEXpand) && (tempArray[i].properties.inE !== undefined)) {
                for (var j = 0; j < tempArray[i].properties.inE[0].vertexProperties.length; j++) {
                    if (paperAlreadyAdded.has(tempArray[i].properties.inE[0].vertexProperties[j].PaperId)) {
                        var newLink = new createLinks(i, paperAlreadyAdded.get(tempArray[i].properties.inE[0].vertexProperties[j].PaperId));
                        linksArray.push(newLink);
                    }
                }
            }
        }
    }

    JSON.stringify(linksArray);
    JSON.stringify(tempArray);
    d3.selectAll("svg > *").remove();
    var mydata=new Set(tempArray);
    for(let item of mydata) console.log(item);
    createGraph(tempArray, linksArray,false);
    d3.select('.context-menu').style('display', 'none');
}

function showReferences(idToEXpand){
    var intial_length = tempArray.length;
    for (var i = 0; i < intial_length; i++) {
        if((tempArray[i].PaperId===idToEXpand) && (!nodeExpandedforRefernce.has(idToEXpand))){
            for(j=0;j<tempArray[i].properties.outE[0].vertexProperties.length;j++){
                var newNode = new createRefernceNode(tempArray[i].properties.outE[0].vertexProperties[j].PaperId, tempArray[i].properties.outE[0].vertexProperties[j].properties);
                var index= tempArray.push(newNode);
                var newLink = new createLinks(i,tempArray.length - 1);
                linksArray.push(newLink);
            }

        }
    }
    JSON.stringify(linksArray);
    JSON.stringify(tempArray);
    d3.selectAll("svg > *").remove();
    var mydata=new Set(tempArray);
    for(let item of mydata) console.log(item);
    createGraph(tempArray, linksArray,false);
    d3.select('.context-menu').style('display', 'none');
}

function downloadGraphAsSVG() {
    try {
        var isFileSaverSupported = !!new Blob();
    } catch (e) {
        alert("blob not supported");
    }

    var graphDwn = d3.select("svg")
        .attr("version", 1.1)
        .attr("xmlns", "http://www.w3.org/2000/svg")
        .node().parentNode.innerHTML;


    var blob = new Blob([graphDwn], {type: "image/svg+xml"});
    saveAs(blob, "scholarlyNetwork.svg");

}

function downloadGraphAsData() {
    var data=JSON.stringify(dataArray);
    var blob_json = new Blob([data], { type: 'text/data;charset=utf-8;' });
    saveAs(blob_json,"data.json");
}

