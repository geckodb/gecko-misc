var processedArray=[[]];
d3.json("../JSON/temp.json",processedArray); //temp.json holds the format required to map d3 objects **DO NOT DELETE TEMP.JSON**
var width = 1050,
    height = 620;
var nodeAdded= new Set();
var fullscreen=true;
var dataLoaded=false;
var activeTabIndex;

var svg=new Array();
 svg[0]=d3.select("#paperGraphArea")
    .append("svg")
     .attr("id","paperGraphArea0")
    .attr("width",width)
    .attr("height",height);



var paperMenuItems = ["Show more info","Remove","Authorship","Domain", "Hosting", "Publishing", "Cited By", "Add tag", "Co-citation", "Bibliographic Coupling", "References"];
var authorMenuItems=["Co-authorship", "Membership","Remove"];
var institutionMenuItems=["Papers","Remove"];
var venueMenuItems=["Papers","Remove"];
var publicationMenuItems=["Papers"];
var FOSMenuItems=["Papers","Remove"];
var selectedSVG;


var force = d3.layout.force()
    .distance(100)
    .linkDistance([150])
    .charge(-500)
    .size([width, height])
    .gravity(0.1)
    .alpha(0);

//	Define the div for the tooltip
var tooltip = d3.select("body")
    .append("div")
    .attr("class", "tooltip")
    .style("opacity", 0);

function poplateClickedNode(nodeId,dataArray) {
    var ulItems=document.getElementsByClassName("workspaceTab");
    var liItems=ulItems[0].getElementsByTagName("li");
    var svgList=d3.selectAll("svg");

    for(var i=0;i<liItems.length;i++){
        if(liItems[i].className==="active"){
            activeTabIndex=i;
            var htmlObj=$(liItems[i].innerHTML);
            selectedSVG=svg[i];
        }
    }

    for(let i=0;i<dataArray.length;i++){
        if(nodeId===dataArray[i]._id){
            processedArray[activeTabIndex].push(dataArray[i]);
            //nodeAdded.add(nodeId);
            //d3.selectAll("svg > *").remove();
            var idName="#"+svgList[0][activeTabIndex].getAttribute("id");
           $(idName).empty();
            createGraph(processedArray[activeTabIndex],linksArray[activeTabIndex],false);
            break;
        }
    }
}

function getActiveTabIndex() {
    var tabIndex;
    var ulItems=document.getElementsByClassName("workspaceTab");
    var liItems=ulItems[0].getElementsByTagName("li");

    for(var i=0;i<liItems.length;i++){
        if(liItems[i].className==="active"){
            tabIndex=i;
            break;
        }
    }
    return tabIndex;
}

function createGraph(nodes, links, drawnodesOnly,noArrowhead) {
    var paperId="";
    var srcjgId="";
    var scrIdPaperIndex="";
    var fromAuthor=false;
    var authorSrcId="";
    $("#graphArea").css("cursor","wait");
//To form arrowhead
    if(noArrowhead){
        //do nothing
    }else{
        selectedSVG.append("defs").selectAll("marker")
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
    }

    force.nodes(nodes)
        .links(links)
        .start();

    var link = selectedSVG.selectAll("link")
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

//Toggle stores whether the highlighting is on
    var toggle = 0;

//Create an array logging what is connected to what
    var linkedByIndex = {};
    for (i = 0; i < nodes.length; i++) {
        linkedByIndex[i + "," + i] = 1;
    };
    links.forEach(function (d) {
        linkedByIndex[d.source.index + "," + d.target.index] = 1;
    });

//This function looks up whether a pair are neighbours
    function neighboring(a, b) {
        return linkedByIndex[a.index + "," + b.index];
    }

    function connectedNodes() {
        if (toggle == 0) {
            //Reduce the opacity of all but the neighbouring nodes
            d = d3.select(this).node().__data__;
            node.style("opacity", function (o) {
                return neighboring(d, o) | neighboring(o, d) ? 1 : 0.1;
            });

            link.style("opacity", function (o) {
                return d.index==o.source.index | d.index==o.target.index ? 1 : 0.1;
            });

            edgelabels.style("opacity",function (o) {
                return d.index==o.source.index | d.index==o.target.index ? 1 : 0.1;
            });
            toggle = 1;
        } else {
            //Reset opacity to 1
            node.style("opacity", 1);
            link.style("opacity", 1);
            edgelabels.style("opacity",1);
            toggle = 0;
        }
    }

    var edgepaths = selectedSVG.selectAll(".edgepath")
        .data(links)
        .enter()
        .append('path')
        .attr({'d': function(d) { return 'M '+d.source.x+' '+d.source.y+' L '+ d.target.x +' '+d.target.y},
            'class':'edgepath',
            'fill-opacity':0,
            'stroke-opacity':0,
            'id':function(d,i) { return 'edgepath'+i;}})
        .style("pointer-events", "none");

    var edgelabels = selectedSVG.selectAll(".edgelabel")
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
            if((d.target._source.vType===vertexType.PAPER)||(d.target._source.vType==="cites")){
                return "cited by"
            }else if(d.target._source.vType===vertexType.AUTHOR){
                return "author"
            }else if(d.target._source.vType===vertexType.ORG){
                return "affiliation"
            } else if(d.target._source.vType==="reference"){
                return "refers to"
            }else if(d.target._source.vType===vertexType.FOS){
                return "fos"
            }else if(d.target._source.vType===vertexType.PUBLICATION){
                return "publisher"
            }else if(d.target._source.vType===vertexType.VENUE){
                return "venue"
            }
        });


    var node = selectedSVG.selectAll("node")
        .data(nodes)
        .enter().append("g")
        .attr("class", "node")
        .call(node_drag)
        .on("dblclick",connectedNodes);


    node.append("image")
        .attr("xlink:href", function (d) {
            if((d._source.vType==="paper") || (d._source.vType==="reference")||(d._source.vType==="cites")) {
                return "http://icons.iconarchive.com/icons/pelfusion/long-shadow-media/512/Document-icon.png"
            }
            else if (d._source.vType==="author"){
                return "http://www.clker.com/cliparts/3/V/U/m/W/U/admin-button-icon-md.png"
            }
            else if(d._source.vType==="org"){
                return "http://www.freeiconspng.com/uploads/institution-icon-15.png"
            }
            else if(d._source.vType==="fos"){
                return "https://www.sas.com/content/dam/SAS/en_us/image/sas-com/icons/navmenu/universities-icon.png/_jcr_content/renditions/cq5dam.thumbnail.140.100.png"
            }
            else if(d._source.vType===vertexType.PUBLICATION){
                return "http://www.projecttejaswini.com/WCD/magazine.png"
            }
            else if(d._source.vType===vertexType.VENUE){
                return "http://www.claudiacasillasmusic.com/assets/img/mappin.ico"
            }
        })
        .attr("x", -8)
        .attr("y", -8)
        .attr("width",function (d) {
            return 30;
        })
        .attr("height", function (d) {
            return 30;
        })

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
                    if((d._source.vType===vertexType.PAPER)||(d._source.vType===vertexType.CITES))
                        return paperMenuItems;
                    else if((d._source.vType===vertexType.AUTHOR)){
                        return authorMenuItems;
                    }else if(d._source.vType===vertexType.ORG){
                        return institutionMenuItems;
                    }else if(d._source.vType===vertexType.VENUE){
                        return venueMenuItems;
                    }else if(d._source.vType===vertexType.FOS){
                        return FOSMenuItems;
                    }

                }).enter()
                .append('li')
                .on('click', function (d) {
                    activeTabIndex=getActiveTabIndex();
                    if((d==="Authorship")){

                        if((processedArray[activeTabIndex][scrIdPaperIndex]._source.vType===vertexType.CITES)&&(processedArray[activeTabIndex][selectedIndex]._source.venuePaper===undefined)){
                            var url="http://localhost:9200/janusgraph_vertexes/_search?q=vType:paper AND jgId="+processedArray[activeTabIndex][scrIdPaperIndex]._id;
                            d3.json(url,function (error,json){
                                if (error) throw error;
                                if(json.hits.hits.length==1){
                                    processedArray[activeTabIndex][scrIdPaperIndex]._source=json.hits.hits[0]._source;

                                    showAuthors(paperId,srcjgId,processedArray[activeTabIndex],activeTabIndex);
                                    paperExpanded.add(paperId);

                                }
                            });
                        }else{
                            showAuthors(paperId,srcjgId,processedArray[activeTabIndex],activeTabIndex);
                            paperExpanded.add(paperId);
                        }
                        d3.select('.context-menu').style('display', 'none');
                    }else if(d==="Cited By"){
                        showCitations(targetId,srcjgId,scrIdPaperIndex,processedArray);
                        d3.select('.context-menu').style('display', 'none');
                    }else if(d==="References"){
                        showReferences(paperId);
                        d3.select('.context-menu').style('display', 'none');
                        nodeExpandedforRefernce.add(paperId);
                    }else if(d==="Membership"){
                        if(fromAuthor){
                            showInstitutionFromAuthor(paperId,processedArray);
                            d3.select('.context-menu').style('display', 'none');
                            fromAuthor=false;

                        }else{

                            showInstitution(authorSrcId,authorName,index,processedArray);
                            d3.select('.context-menu').style('display', 'none');
                        }
                    }
                    else if(d==="Domain"){
                        if((processedArray[scrIdPaperIndex]._source.vType===vertexType.CITES)&&(processedArray[scrIdPaperIndex]._source.fosPaper===undefined)){
                            var url="http://localhost:9200/janusgraph_vertexes/_search?q=vType:paper AND jgId="+processedArray[scrIdPaperIndex]._id;
                            d3.json(url,function (error,json){
                                if (error) throw error;
                                if(json.hits.hits.length==1){
                                    processedArray[scrIdPaperIndex]._source=json.hits.hits[0]._source;
                                    showFOS(paperId,scrIdPaperIndex,processedArray);
                                    dataLoaded=true;
                                }
                            });
                        }else{
                            showFOS(paperId,scrIdPaperIndex,processedArray);
                        }
                        d3.select('.context-menu').style('display', 'none');
                    }
                    else if(d==="Publishing"){
                        if((processedArray[scrIdPaperIndex]._source.vType===vertexType.CITES)&&(processedArray[scrIdPaperIndex]._source.publisherPaper===undefined)){
                            var url="http://localhost:9200/janusgraph_vertexes/_search?q=vType:paper AND jgId="+processedArray[scrIdPaperIndex]._id;
                            d3.json(url,function (error,json){
                                if (error) throw error;
                                if(json.hits.hits.length==1){
                                    processedArray[scrIdPaperIndex]._source=json.hits.hits[0]._source;
                                    showPublication(paperId,scrIdPaperIndex,processedArray);
                                    dataLoaded=true;
                                }
                            });
                        }else{
                            showPublication(paperId,scrIdPaperIndex,processedArray);
                        }
                        d3.select('.context-menu').style('display', 'none');
                    }
                    else if(d==="Hosting"){
                        if((processedArray[scrIdPaperIndex]._source.vType===vertexType.CITES)&&(processedArray[selectedIndex]._source.venuePaper===undefined)){
                            var url="http://localhost:9200/janusgraph_vertexes/_search?q=vType:paper AND jgId="+processedArray[scrIdPaperIndex]._id;
                            d3.json(url,function (error,json){
                                if (error) throw error;
                                if(json.hits.hits.length==1){
                                    processedArray[scrIdPaperIndex]._source=json.hits.hits[0]._source;
                                    showVenue(paperId,scrIdPaperIndex,processedArray);

                                }
                            });
                        }else{
                            showVenue(paperId,scrIdPaperIndex,processedArray);
                        }
                        d3.select('.context-menu').style('display', 'none');

                    }
                    else if(d=="Show more info"){
                        if(processedArray[scrIdPaperIndex]._source.vType===vertexType.CITES){
                            var url="http://localhost:9200/janusgraph_vertexes/_search?q=vType:paper AND jgId="+processedArray[scrIdPaperIndex]._id;
                            d3.json(url,function (error,json){
                                if (error) throw error;
                                if(json.hits.hits.length==1){
                                    processedArray[scrIdPaperIndex]._source=json.hits.hits[0]._source;
                                    showCompleteDetails(selectedIndex,processedArray);

                                }
                            });
                        }else{
                            showCompleteDetails(selectedIndex,processedArray);
                        }
                        d3.select('.context-menu').style('display', 'none');
                    }else if(d=="Co-authorship"){
                        // if(processedArray[scrIdPaperIndex]._source.vType===vertexType.PAPER){
                        showCoAuthorship(authorSrcId,selectedIndex,processedArray);
                        /*}else{
                            showCoAuthorship(selectedIndex,processedArray);
                        }*/

                    }
                    else if(d=="Add tag"){
                        addTag();
                        d3.select('.context-menu').style('display', 'none');
                    }else if(d=="Remove"){
                        if(processedArray[selectedIndex]._source.vType===vertexType.PAPER){
                            if((nodeAdded.has(processedArray[selectedIndex]._id))){
                                nodeAdded.delete(processedArray[selectedIndex]._id);
                            }
                        }else if(processedArray[selectedIndex]._source.vType===vertexType.AUTHOR){
                            if((authorAlreadyAdded.has(processedArray[selectedIndex]._source.author))){
                                authorAlreadyAdded.delete(processedArray[selectedIndex]._source.author)
                            }
                        }
                        removeNodeAndLinks(selectedIndex,processedArray,linksArray);
                        d3.select('.context-menu').style('display', 'none');
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
            console.log(d);
            if(d._source.vType===vertexType.PAPER){
                paperId = d._id;
                srcjgId=d._source.jgId;
                scrIdPaperIndex=d.index;
            }else if(d._source.vType===vertexType.AUTHOR){
                if(d.createdNode!==undefined){
                    var authorSrcId=d.authorId;
                    var authorName=d._source.author;
                    var index=d.index;
                }else{
                    fromAuthor=true;
                }
            }else if(d._source.vType===vertexType.CITES){
                var targetId=d._source.tgtId;
                srcjgId=d._source.jgId;
                scrIdPaperIndex=d.index;
            }
            paperId = d._id;
            var selectedIndex=d.index;

        })

        //To display tooltip
        .on("mouseover", function (d) {
            //	Define the div for the tooltip
            if((d._source.vType===vertexType.PAPER)){
                tooltip.transition()
                    .duration(200)
                    .style("opacity", 0.9);
                tooltip.html("<b>Title : </b>"+d._source.title + "<br/>"+
                    "<b>Authors : </b>"+d._source.authors + "<br/>"+
                    "<a> To view more details use show more info option ...</a>"+ "<br/>")
                    .style("left", (d.x+20) + "px")
                    .style("top", (d.y) + "px");

            }
            else if(d._source.vType==="cites"){
                tooltip.transition()
                    .duration(200)
                    .style("opacity", 0.9);
                tooltip.html("<b>Title : </b>"+d._source.title + "<br/>"+
                    "<a> To view more details use show more info option ...</a>"+ "<br/>")
                    .style("left", (d.x+20) + "px")
                    .style("top", (d.y) + "px");
            }
            else if(d._source.vType===vertexType.AUTHOR){
                tooltip.transition()
                    .duration(200)
                    .style("opacity", 0.9);
                tooltip.html(d._source.author + "<br/>")
                    .style("left", (d3.event.pageX) + "px")
                    .style("top", (d3.event.pageY - 30) + "px");
            }
            else if(d._source.vType===vertexType.FOS){
                tooltip.transition()
                    .duration(200)
                    .style("opacity", 0.9);
                tooltip.html(d._source.fos + "<br/>")
                    .style("left", (d3.event.pageX) + "px")
                    .style("top", (d3.event.pageY - 30) + "px");
            }
            else if(d._source.vType===vertexType.VENUE){
                tooltip.transition()
                    .duration(200)
                    .style("opacity", 0.9);
                tooltip.html(d._source.venue + "<br/>")
                    .style("left", (d3.event.pageX) + "px")
                    .style("top", (d3.event.pageY - 30) + "px");
            }
            else if(d._source.vType===vertexType.ORG){
                tooltip.transition()
                    .duration(200)
                    .style("opacity", 0.9);
                tooltip.html(d._source.org + "<br/>")
                    .style("left", (d3.event.pageX) + "px")
                    .style("top", (d3.event.pageY - 30) + "px");
            }
            else if(d._source.vType===vertexType.PUBLICATION){
                tooltip.transition()
                    .duration(200)
                    .style("opacity", 0.9);
                tooltip.html(d._source.publisher + "<br/>")
                    .style("left", (d3.event.pageX) + "px")
                    .style("top", (d3.event.pageY - 30) + "px");
            }
        })
        .on( 'mouseenter', function(d) {
            // select element in current context

            /* node.append("circle")
            .attr("r","20")
                 .attr("cx",function (d){return 0;})
                 .attr("cy",function (d){return 0;})
                 .text(function (d) {
                     return "remove";
                 });*/


            d3.select( this )
                .transition()
                .attr("x", function(d) { return -30;})
                .attr("y", function(d) { return -30;})
                .attr("height", 60)
                .attr("width", 60)
        })
        // set back
        .on( 'mouseleave', function() {
            d3.select( this )
                .transition()
                .attr("x", function(d) { return -5;})
                .attr("y", function(d) { return -5;})
                .attr("height", 30)
                .attr("width", 30);
        })
        //to disbale tooltip
        .on("mouseout", function (d) {
            tooltip.transition()
                .duration(200)
                .style("opacity", 0);
        });


    node.append("title")
        .attr("dx", 20)
        .attr("dy", ".35em");


    node.append("text")
        .attr("dx", 20)
        .attr("dy", ".35em")
        .text(function (d) {
            if((d._source.vType===vertexType.PAPER)||(d._source.vType==="cites")){
                var temp=d._source.title;
                return temp.substring(0,15)+"...";
            }
            else if(d._source.vType===vertexType.AUTHOR){
                return d._source.author;
            }
            else if(d._source.vType==="reference"){
                return d._source.title;
            }
            else if(d._source.vType===vertexType.ORG){
                return d._source.org;
            }else if(d._source.vType===vertexType.FOS){
                return d._source.fos;
            }else if(d._source.vType===vertexType.PUBLICATION){
                return d._source.publisher;
            }else if(d._source.vType===vertexType.VENUE){
                return d._source.venue;
            }
        });


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

    $("#graphArea").css("cursor","default");
}

function resize() {
    if(fullscreen){
        $("#graphArea").removeClass("startSize").addClass("newSize");
        svg.attr("width",1500)
            .attr("height",1000);
        fullscreen=false;
    }
    else{
        $("#graphArea").removeClass("newSize").addClass("startSize");
        svg.attr("width",1050)
            .attr("height",600);
        fullscreen=true;
    }
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
    saveAs(blob, "scholarlyNetwork.html");

}


function downloadGraphAsData() {
    var data=JSON.stringify(dataArray);
    var blob_json = new Blob([data], { type: 'text/data;charset=utf-8;' });
    saveAs(blob_json,"data.json");
}

function clearSVG() {
    d3.selectAll("svg > *").remove();
    processedArray=new Array();
    linksArray=new Array();
    nodeAdded= new Set();
    authorAlreadyAdded=new Map();
    publicationAlreadyAdded=new Map();
    venueAlreadyAdded=new Map();
    fosAlreadyAdded=new Map();
    paperAlreadyAded=new Map();
    paperExpanded= new Set();
    nodeExpandedforRefernce = new Set();
    instituteAlreadyAdded=new Map();

}


var vertexType={
    PAPER:"paper",
    AUTHOR:"author",
    ORG:"org",
    FOS:"fos",
    PUBLICATION:"publication",
    VENUE:"venue",
    CITES:"cites",
    REFERENCES:""
}
