var width = 1050,
    height = 580;
var tempArray=new Array();
var dataArray = new Array();
var linksArray = new Array();
var authorDetails=new Map();
var paperExpanded= new Set();
var svg = d3.select("#paperGraphArea")
    .append("svg")
    .attr("width",width)
    .attr("height",height);


var menuItems = ["Authorship", "Co-word", "Bibliographic"];
var authorMenuItems=["Co-word", "Bibliographic"];
var fullscreen=true;


var force = d3.layout.force()
    .distance(100)
    .linkDistance([150])
    .charge(-500)
    .size([width, height])
    .gravity(0.1)
    .alpha(0);



d3.json("../JSON/QueryData.json", function (error, json) {
    if (error) throw error;

    dataArray = json.nodes;
    d3.json("../JSON/temp.json",tempArray);
    linksArray = json.links;

//Replacing paperId values in source and target to indexes of dataArray as required by d3, to form graph
    /*  for (var k = 0; k < linksArray.length; k++) {
          for (l = 0; l < dataArray.length; l++) {
              if (linksArray[k].source === dataArray[l].PaperId) {
                  linksArray[k].source = l;
              }
              else if (linksArray[k].target === dataArray[l].PaperId) {
                  linksArray[k].target = l;
              }
          }
      }*/

    //createGraph(json.nodes, linksArray, true);


    //Pagination code

    $(document).ready(function(){

        var $pagination = $('#pagination'),
            totalRecords = 0,
            records = [],
            displayRecords = [],
            recPerPage = 6,
            page = 1,
            totalPages = 0;

        records=dataArray
        console.log(records);
        totalRecords = records.length;
        totalPages = Math.ceil(totalRecords / recPerPage);
        apply_pagination();

        function generate_table() {
            $('#searchResults').html('');
            var doc=document.getElementById("searchResults");
            for (var i = 0; i < displayRecords.length; i++) {
                var tr = document.createElement("tr");
                var td=document.createElement("td");

                a = document.createElement("a");
                a.setAttribute("id",displayRecords[i].PaperId)
                a.onclick=function (d) {

                    //alert(a.getAttribute("id"))
                    poplateClickedNode(d.path[1].getAttribute("id"));
                }
                linebreak= document.createElement("br");
                h5= document.createElement("b");
                handler_a= td.appendChild(a);
                h5.innerText=displayRecords[i].title;
                handler_a.appendChild(h5);
                p=document.createElement("p");

                handler_p=td.appendChild(p); //change to doc to keep it out of a tag
                b= document.createElement("b");
                b.innerText="Citation Count :  ";

                span=document.createElement("span");
                span.innerText=displayRecords[i].citedby;
                handler_p.appendChild(b);
                handler_p.appendChild(span);

                b1= document.createElement("b");
                b1.innerText=" Reference Count : ";
                span1=document.createElement("span");
                span1.innerText=displayRecords[i].reference_count;
                handler_p.append(' ');
                handler_p.appendChild(b1);
                handler_p.appendChild(span1);
                td.appendChild(linebreak);
                tr.appendChild(td);
                doc.appendChild(tr);
            }
        }
        //calculates start and end indices and sends set of records to display
        function apply_pagination() {
            $pagination.twbsPagination({
                totalPages: totalPages,
                visiblePages: 6,
                onPageClick: function (event, page) {
                    displayRecordsIndex = Math.max(page - 1, 0) * recPerPage;
                    endRec = (displayRecordsIndex) + recPerPage;
                    displayRecords = records.slice(displayRecordsIndex, endRec);
                    generate_table();
                }
            });
        }
    });
});

function poplateClickedNode(nodeId) {
    for(let i=0;i<dataArray.length;i++){
        if(nodeId==dataArray[i].PaperId){
            tempArray.push(dataArray[i]);
            d3.selectAll("svg > *").remove();

            createGraph(tempArray,linksArray,false);
            break;
        }
    }
}



function createGraph(nodes, links, drawnodesOnly) {
    var label=new Array();

    if(!drawnodesOnly) {

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
                if(d.source.type==="paper"){
                    return "cites"
                }else{
                    return "author"
                }

            });
    }
    else{

        force.nodes(nodes)
            .start();

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
            node_tick(); // this is the key to make it work together with updating both px,py,x,y on d !
        }

        function drag_end(d, i) {
            d.fixed = true; // set the node to fixed, so the force doesn't include the node in its auto positioning
            node_tick();
            force.resume();
        }
    }



    var node = svg.selectAll("node")
        .data(nodes)
        .enter().append("g")
        .attr("class", "node")
        .call(node_drag);

    node.append("image")
        .attr("xlink:href", function (d) {
            if(d.type==="paper"){
                return "http://icons.iconarchive.com/icons/pelfusion/long-shadow-media/512/Document-icon.png"
                //return "../Images/Document.ico";
            }
            else if (d.type==="author"){
                return "http://www.clker.com/cliparts/3/V/U/m/W/U/admin-button-icon-md.png"
                //return "../Images/author.png";
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
        /*  .style("stroke-width", "10")*/
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
                    if(d.type==="paper")
                        return menuItems;
                    else
                        return authorMenuItems;
                }).enter()
                .append('li')
                .on('click', function (d) {
                    console.log("context menu-" +d);
                    if((d==="Authorship")){
                        parseData(paperId);
                        paperExpanded.add(paperId);
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
            if(d.type==="paper"){
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
            if(d.type==="paper"){
                return d.title;
            }
            else if(d.type==="author"){
                return d.authors;
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

function createLinks(source, target) {
    this.source = source;
    this.target = target;
}

function parseData(idToEXpand) {

    var intial_length = tempArray.length;
    for (var i = 0; i < intial_length; i++) {
        if(tempArray[i].PaperId===idToEXpand){
            for (var j = 0; j < tempArray[i].authors.length; j++) {
                if(!authorDetails.has(tempArray[i].authors[j].name)) {
                    var newNode = new createAuthorNode(tempArray[i].PaperId, tempArray[i].authors[j].name)
                    var index= tempArray.push(newNode);
                    authorDetails.set(tempArray[i].authors[j].name,index-1);
                    var newLink = new createLinks(tempArray.length - 1, i);
                    linksArray.push(newLink);
                }

            }
        }
    }

    for (var i = 0; i < intial_length; i++) {
        if(tempArray[i].PaperId!==idToEXpand) {
            for (var j = 0; j < tempArray[i].authors.length; j++) {
                if(authorDetails.has(tempArray[i].authors[j].name)){
                    var newLink = new createLinks(authorDetails.get(tempArray[i].authors[j].name), i);
                    linksArray.push(newLink);
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

