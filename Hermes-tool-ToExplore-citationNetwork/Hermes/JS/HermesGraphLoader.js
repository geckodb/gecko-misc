var processedArray=[[]];
var sliderArray=[[]];
d3.json("../JSON/temp.json",processedArray); //temp.json holds the format required to map d3 objects **DO NOT DELETE TEMP.JSON**
var width, height
var nodeAdded= new Set();
var fullscreen=true;
var dataLoaded=false;
var activeTabIndex;
var yearFilteredArray=new Array();
var svg=new Array();
var force=new Array();
var node=new Array();
var link=new Array();
var edgepaths=new Array();
var edgelabels=new Array();
var linkedByIndex = {};

var margin = {
        top: 40,
        right: 20,
        bottom: 40,
        left: -10
    },
    width = 1300 - margin.right - margin.left,
    height = 1600 - margin.top - margin.bottom;

var sliderEnabled;
 svg[0]=d3.select("#tabArea0")
    .append("svg")
     .attr("id","paperGraphArea0")
     //.attr("viewBox", "0 0 100% auto")
    .attr("width",width)
    .attr("height",height)
     .attr("transform", "translate(" + margin.left + "," + margin.top + ")");
    // .attr({"width":"100%","height":"auto"})
   //  .attr("overflow","scroll");

var paperMenuItems = ["Show more info","Remove","Show Authors", "References", "Hosting", "Publishing", "Citations"]; //,"Domain", "Co-citation", "Bibliographic Coupling"
var authorMenuItems=["Co-authorship", "Papers Authored","Citing non-coauthors","Remove"];
var institutionMenuItems=["Papers","Remove"];
var venueMenuItems=["Papers","Remove"];
var publicationMenuItems=["Papers published","Remove"];
var FOSMenuItems=["Papers","Remove"];
var selectedSVG;


 force[0] = d3.layout.force()
    .distance(100)
    .linkDistance([150])
    .charge(-200)
     .chargeDistance(1400)
    .size([1050,1000])
    .gravity(0.1)
     .friction(0.8)
    .alpha(0);


//	Define the div for the tooltip
var tooltip = d3.select("body")
    .append("div")
    .attr("class", "tooltip")
    .style("opacity", 0);

function poplateClickedNode(nodeId,dataArray,populateAllNodes) {
    var ulItems=document.getElementsByClassName("workspaceTab");
    var liItems=ulItems[0].getElementsByTagName("li");
    var svgList=d3.selectAll("svg");
    var pos;
    var buildGraph=false;
    for(var i=0;i<liItems.length;i++){
        if(liItems[i].className==="active"){
            activeTabIndex=i;
            var htmlObj=$(liItems[i].innerHTML);
            selectedSVG=svg[i];
        }
    }
    if(!populateAllNodes){
        for(let i=0;i<dataArray.length;i++){
            if(nodeId===dataArray[i]._id){
               // if( (!paperAlreadyAdded[activeTabIndex].has(dataArray[i]._source.PAPER_ID))||(!authorAlreadyAdded[activeTabIndex].has(dataArray[i]._source.AUTHOR_ID))){
                if(dataArray[i]._source.vType===vertexType.PAPER){
                    if(!paperAlreadyAdded[activeTabIndex].has(dataArray[i]._source.PAPER_ID)){
                        pos=processedArray[activeTabIndex].push(dataArray[i]);
                        paperAlreadyAdded[activeTabIndex].set(dataArray[i]._source.PAPER_ID,pos-1);
                        _LTracker.push({
                            'method':'poplateClickedNode',
                            'text': 'PaperNode',
                            'PaperNode': {
                                'PaperID': dataArray[i]._source.PAPER_ID,
                                'Title':dataArray[i]._source.Title
                            },
                            'tag':'PopulateNode-paper'
                        });
                        buildGraph=true;
                    }
                }else if(dataArray[i]._source.vType===vertexType.AUTHOR){
                    if(!authorAlreadyAdded[activeTabIndex].has(dataArray[i]._source.AUTHOR_NAME)){
                        pos=processedArray[activeTabIndex].push(dataArray[i]);
                        authorAlreadyAdded[activeTabIndex].set(dataArray[i]._source.AUTHOR_NAME,pos-1);
                        _LTracker.push({
                            'method':'poplateClickedNode',
                            'text': 'AuthorNode',
                            'AuthorNode': {
                                'AuthorId': dataArray[i]._source.AUTHOR_ID,
                                'AuthorName':dataArray[i]._source.AUTHOR_NAME
                            },
                            'tag':'PopulateNode-author'
                        });
                        buildGraph=true;
                    }
                }else if(dataArray[i]._source.vType===vertexType.PUBLICATION){
                    if(!publicationAlreadyAdded[activeTabIndex].has(dataArray[i]._source.JOURNAL_NAME)){
                        pos=processedArray[activeTabIndex].push(dataArray[i]);
                        publicationAlreadyAdded[activeTabIndex].set(dataArray[i]._source.JOURNAL_NAME,pos-1);
                        _LTracker.push({
                            'method':'poplateClickedNode',
                            'text': 'JournalNode',
                            'JournalNode': {
                                'JournalId': dataArray[i]._source.JOURNAL_ID,
                            },
                            'tag':'PopulateNode-journal'
                        });
                        buildGraph=true;
                    }
                }else if(dataArray[i]._source.vType===vertexType.VENUE){
                    if(!publicationAlreadyAdded[activeTabIndex].has(dataArray[i]._source.VENUE_NAME)){
                        pos=processedArray[activeTabIndex].push(dataArray[i]);
                        publicationAlreadyAdded[activeTabIndex].set(dataArray[i]._source.VENUE_NAME,pos-1);
                        _LTracker.push({
                            'method':'poplateClickedNode',
                            'text': 'VenueNode',
                            'VenueNode': {
                                'VenueId': dataArray[i]._source.VENUE_ID,
                            },
                            'tag':'PopulateNode-venue'
                        });
                        buildGraph=true;
                    }
                }
                    if(buildGraph) {
                        var idName = "#" + svgList[0][activeTabIndex].getAttribute("id");
                        $(idName).empty();
                        createGraph(processedArray[activeTabIndex], linksArray[activeTabIndex], false, false, activeTabIndex);
                        break;
                    }
            }
        }
    }else{
        for(let i=0;i<dataArray.length;i++){
            if(dataArray[i]._source.PAPER_ID!==undefined){
                if(!paperAlreadyAdded[activeTabIndex].has(dataArray[i]._source.PAPER_ID)){
                    _LTracker.push({
                        'method':'poplateClickedNode',
                        'text': 'PaperNode',
                        'PaperNode': {
                            'PaperID': dataArray[i]._source.PAPER_ID,
                            'Title':dataArray[i]._source.Title
                        },
                        'tag':'PopulateNode-paper'
                    });
                    processedArray[activeTabIndex].push(dataArray[i]);
                    paperAlreadyAdded[activeTabIndex].set(dataArray[i]._source.PAPER_ID,pos-1)
                }
            }else if(dataArray[i]._source.AUTHOR_ID!==undefined){
                if(!authorAlreadyAdded[activeTabIndex].has(dataArray[i]._source.AUTHOR_NAME)){
                    _LTracker.push({
                        'method':'poplateClickedNode',
                        'text': 'AuthorNode',
                        'AuthorNode': {
                            'AuthorId': dataArray[i]._source.AUTHOR_ID,
                            'AuthorName':dataArray[i]._source.AUTHOR_NAME
                        },
                        'tag':'PopulateNode-author'
                    });

                    processedArray[activeTabIndex].push(dataArray[i]);
                    authorAlreadyAdded[activeTabIndex].set(dataArray[i]._source.AUTHOR_NAME,pos-1)
                }
            }
      }
        var idName="#"+svgList[0][activeTabIndex].getAttribute("id");
        $(idName).empty();
        createGraph(processedArray[activeTabIndex],linksArray[activeTabIndex],false,false,activeTabIndex);
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

function checkYear(nodesInActiveTab) {
    var activetab=getActiveTabIndex();
    if(nodesInActiveTab._source.vType===vertexType.PAPER)
    {
        if ((nodesInActiveTab._source.Year <= document.getElementById("sliderValue").value)) {
            sliderArray[activetab].push(nodesInActiveTab);
            return true;
        } else {
            return false;
        }
    }else{
        sliderArray[activetab].push(nodesInActiveTab);
        return true;
    }
}

function neighbors(a, b) {
    return linkedByIndex[a.index + "," + b.index];
}

function sliderMoved() {
    if(sliderEnabled){
    var val = document.getElementById("sliderValue").value;
    var selectedYearValue = document.getElementById("selectedyear");
    selectedYearValue.innerText = val;
    //checkYear(val);
    var inTab = getActiveTabIndex();
    var nodesInActiveTab = svg[inTab].selectAll(".node");
    var linksInActiveTab=svg[inTab].selectAll(".link");
    var edgeLabels=svg[inTab].selectAll(".edgelabels");

    if(nodesInActiveTab[0].length>0){
        sliderArray[inTab]=new Array();
        node[inTab].style("opacity", function (o) {
            return checkYear(o) ? 1 : 0.05;
        });
        }
    }

    if((linksInActiveTab[0].length>0)||(sliderArray[inTab].length==0)){
        link[inTab].style("opacity",0.001);
    }

    if(edgeLabels[0].length>0){
        edgelabels[inTab].style("opacity",0.001);
        edgepaths[inTab].style("opacity",0.05);
    }
}

function createGraph(nodes, links, drawnodesOnly,noArrowhead,activeTab) {
    var paperId="";
    var srcjgId="";
    var scrIdPaperIndex="";
    var fromAuthor=false;
    var authorSrcId="";
    $("#graphArea").css("cursor","wait");

    var ulItems=document.getElementsByClassName("workspaceTab");
    var liItems=ulItems[0].getElementsByTagName("li");
    var htmlObj=$(liItems[activeTab].innerHTML);
    selectedSVG=svg[activeTab];

    _LTracker.push({
        'method':'createGraph',
        'text': 'graph structure',
        'graphStructure': {
            'nodes': nodes.length,
            'edges': links.length
        },
        'tag':'graphstructure'
    });

//To form arrowhead
    if(noArrowhead){
        //do nothing
    }else{
        selectedSVG
            .append("defs")
            .selectAll("marker")
            .data(["arrowhead"])
            .enter()
            .append("svg:marker")
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

    force[activeTab].nodes(nodes)
        .links(links)
        .start();

     link[activeTab] = selectedSVG.selectAll("link")
         .data(links)
        .enter().append("line")
        .attr("class", function(d) {
            if (d.label !== "coauthor") {
                return "link"
            }
        })
        .attr("marker-end", function(d){
            if(d.label!=="coauthor"){
              return  "url(#arrowhead)"
            }
        })
        .attr('fill', '#ccc')
        .attr('stroke', '#ccc');

    var node_drag = d3.behavior.drag()
        .on("dragstart", drag_start)
        .on("drag", drag_move)
        .on("dragend", drag_end);

    function drag_start(d, i) {
        force[activeTab].stop() // stops the force auto positioning before you start dragging
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
        force[activeTab].resume();
    }

//Toggle stores whether the highlighting is on
    var toggle = 0;

//Create an array logging what is connected to what

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
            node[activeTab].style("opacity", function (o) {
                return neighboring(d, o) | neighboring(o, d) ? 1 : 0.1;
            });

            link[activeTab].style("opacity", function (o) {
                return d.index==o.source.index | d.index==o.target.index ? 1 : 0.1;
            });

            edgelabels[activeTab].style("opacity",function (o) {
                return d.index==o.source.index | d.index==o.target.index ? 1 : 0.1;
            });
            toggle = 1;
        } else {
            //Reset opacity to 1
            node[activeTab].style("opacity", 1);
            link[activeTab].style("opacity", 1);
            edgelabels[activeTab].style("opacity",1);
            toggle = 0;
        }
    }


     edgepaths[activeTab] = selectedSVG.selectAll(".edgepath")
        .data(links)
        .enter()
        .append('path')
        .attr({'d': function(d) { return 'M '+d.source.x+' '+d.source.y+' L '+ d.target.x +' '+d.target.y},
            'class':'edgepath',
            'fill-opacity':0,
            'stroke-opacity':0,
            'id':function(d,i) { return 'edgepath'+i;}})
        .style("pointer-events", "none");

     edgelabels[activeTab] = selectedSVG.selectAll(".edgelabel")
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

    edgelabels[activeTab].append("textPath")
        .attr('xlink:href',function(d,i) {return '#edgepath'+i})
        .style("pointer-events", "none")
        .text(function(d,i){
            return d.label;
            /*if((d.target._source.vType===vertexType.PAPER)||(d.target._source.vType==="cites")){
                return "cited by"
            }else if(d.target._source.vType===vertexType.AUTHOR){
                return "author"
            }else if(d.target._source.vType===vertexType.ORG){
                return "affiliation"
            }else if(d.target._source.vType===vertexType.PUBLICATION){
                return "publisher"
            }else if(d.target._source.vType===vertexType.VENUE){
                return "venue"
            }*/
        });


     node[activeTab] = selectedSVG.selectAll("node")
        .data(nodes)
        .enter().append("g")
        .attr("class", "node")
        .call(node_drag)
        .on("dblclick",connectedNodes);


    node[activeTab].append("image")
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
            return 27;
        })
        .attr("height", function (d) {
            return 27;
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
                    if(d._source.vType===vertexType.PAPER)
                        return paperMenuItems;
                    else if(d._source.vType===vertexType.AUTHOR){
                        return authorMenuItems;
                    }else if(d._source.vType===vertexType.PUBLICATION){
                        return publicationMenuItems;
                    }else if(d._source.vType===vertexType.VENUE){
                        return venueMenuItems;
                    }else if(d._source.vType===vertexType.FOS){
                        return FOSMenuItems;
                    }

                }).enter()
                .append('li')
                .on('click', function (d) {
                    activeTabIndex=getActiveTabIndex();
                    if((d==="Show Authors")){
                            showAuthors(processedArray[activeTab][selectedIndex]._source.PAPER_ID,selectedIndex,processedArray[activeTabIndex],activeTabIndex);
                            paperExpanded.add(paperId);

                        d3.select('.context-menu').style('display', 'none');
                    }else if(d==="Citations"){
                        showCitations(selectedIndex,processedArray[activeTabIndex],activeTabIndex);
                        d3.select('.context-menu').style('display', 'none');
                    }else if(d==="References"){
                        showReferences(selectedIndex,processedArray[activeTabIndex],activeTabIndex);
                        d3.select('.context-menu').style('display', 'none');
                        nodeExpandedforRefernce.add(paperId);
                    }
                    else if(d==="Domain"){
                        if((processedArray[activeTab][scrIdPaperIndex]._source.vType===vertexType.CITES)&&(processedArray[activeTab][scrIdPaperIndex]._source.fosPaper===undefined)){
                            var url="http://localhost:9200/https://mondrian-db.slack.com//_search?q=vType:paper AND jgId="+processedArray[scrIdPaperIndex]._id;
                            d3.json(url,function (error,json){
                                if (error) throw error;
                                if(json.hits.hits.length==1){
                                    processedArray[activeTabIndex][scrIdPaperIndex]._source=json.hits.hits[0]._source;
                                    showFOS(paperId,scrIdPaperIndex,processedArray[activeTabIndex],activeTabIndex);
                                    dataLoaded=true;
                                }
                            });
                        }else{
                            showFOS(paperId,scrIdPaperIndex,processedArray[activeTabIndex],activeTabIndex);
                        }
                        d3.select('.context-menu').style('display', 'none');
                    }
                    else if(d==="Publishing") {
                        if (processedArray[activeTab][scrIdPaperIndex]._source.vType === vertexType.PAPER) {
                            showJournals(scrIdPaperIndex,processedArray[activeTabIndex],activeTabIndex);
                            d3.select('.context-menu').style('display', 'none');
                        }
                    }else if(d==="Papers published"){
                        showPapersPublished(selectedIndex,processedArray[activeTabIndex],activeTabIndex)
                    }
                    else if(d==="Hosting"){
                        if (processedArray[activeTab][scrIdPaperIndex]._source.vType === vertexType.PAPER) {
                            showVenue(scrIdPaperIndex, processedArray[activeTabIndex], activeTabIndex);
                            d3.select('.context-menu').style('display', 'none');
                        }


                    }
                    else if(d=="Show more info"){
                        if(processedArray[activeTab][scrIdPaperIndex]._type===vertexType.CITES){
                            var url="http://localhost:9200/janusgraph_vertexes/_search?q=vType:paper AND _id="+processedArray[activeTab][scrIdPaperIndex]._id;
                            d3.json(url,function (error,json){
                                if (error) throw error;
                                if(json.hits.hits.length==1){
                                    processedArray[activeTab][scrIdPaperIndex]._source=json.hits.hits[0]._source;
                                    showCompleteDetails(selectedIndex,processedArray[activeTabIndex],activeTabIndex);
                                }
                            });
                        }else{
                            showCompleteDetails(selectedIndex,processedArray[activeTabIndex],activeTabIndex);
                        }
                        d3.select('.context-menu').style('display', 'none');
                    }else if(d=="Co-authorship"){
                        showCoAuthorship(selectedIndex,processedArray[activeTabIndex],activeTabIndex);

                    }else if(d=="Papers Authored"){
                        showPapersAuthored(selectedIndex,processedArray[activeTabIndex],activeTabIndex);
                    }else if(d=="Citing non-coauthors"){
                        showAuthorNotCoauthor(selectedIndex,processedArray[activeTabIndex],activeTabIndex);
                    }
                    else if(d=="Add tag"){
                        addTag();
                        d3.select('.context-menu').style('display', 'none');
                    }else if(d=="Remove"){
                        if(processedArray[activeTabIndex][selectedIndex]._source.vType===vertexType.PAPER){
                            if((paperAlreadyAdded[activeTabIndex].has(processedArray[activeTabIndex][selectedIndex]._source.PAPER_ID))){
                                paperAlreadyAdded[activeTabIndex].delete(processedArray[activeTabIndex][selectedIndex]._source.PAPER_ID);
                            }
                        }else if(processedArray[activeTabIndex][selectedIndex]._source.vType===vertexType.AUTHOR){
                            if((authorAlreadyAdded[activeTabIndex].has(processedArray[activeTabIndex][selectedIndex]._source.AUTHOR_NAME))){
                                authorAlreadyAdded[activeTabIndex].delete(processedArray[activeTabIndex][selectedIndex]._source.AUTHOR_NAME)
                            }
                        }
                        removeNodeAndLinks(selectedIndex,processedArray[activeTabIndex],linksArray[activeTabIndex],activeTabIndex);
                        d3.select('.context-menu').style('display', 'none');
                    }
                })
                .text(function (d) { return d;});


            d3.select('.context-menu').style('display', 'none');
            // show the context menu
            d3.select('.context-menu')
                .style('left',d3.event.pageX - 2 + 'px')
                .style('top', d3.event.pageY - 2 + 'px')
                .style('display', 'block');
            d3.event.preventDefault();

            if(d._source.vType===vertexType.PAPER){
                paperId = d._id;
                //srcjgId=d._source.jgId;
                scrIdPaperIndex=d.index;
            }else if(d._source.vType===vertexType.AUTHOR){
                if(d.createdNode!==undefined){
                    var authorSrcId=d._id;
                   // var authorName=d._source.AUTHOR_NAME;
                    var index=d.index;
                }else{
                    fromAuthor=true;
                }
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
                tooltip.html("<b>Title : </b>"+d._source.Title + "<br/>"+
                    "<b>Authors : </b>"+d._source.Authors + "<br/>"+
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
                tooltip.html(d._source.AUTHOR_NAME + "<br/>")
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
                tooltip.html(d._source.VENUE_NAME + "<br/>")
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
                tooltip.html(d._source.JOURNAL_NAME + "<br/>")
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
                .attr("height", 27)
                .attr("width", 27);
        })
        //to disbale tooltip
        .on("mouseout", function (d) {
            tooltip.transition()
                .duration(200)
                .style("opacity", 0);
        });


    node[activeTab].append("Title")
        .attr("dx", 20)
        .attr("dy", ".35em");


    node[activeTab].append("text")
        .attr("dx", 20)
        .attr("dy", ".35em")
        .text(function (d) {
            if((d._source.vType===vertexType.PAPER)||(d._source.vType==="cites")){
                var temp=d._source.Title;
                return temp.substring(0,15)+"...";
            }
            else if(d._source.vType===vertexType.AUTHOR){
                return d._source.AUTHOR_NAME;
            }
            else if(d._source.vType==="reference"){
                return d._source.title;
            }
            else if(d._source.vType===vertexType.ORG){
                return d._source.org;
            }else if(d._source.vType===vertexType.FOS){
                return d._source.fos;
            }else if(d._source.vType===vertexType.PUBLICATION){
                return d._source.JOURNAL_NAME;
            }else if(d._source.vType===vertexType.VENUE){
                return d._source.VENUE_NAME;
            }
        });


    if(!drawnodesOnly)
        force[activeTab].on("tick",tick );
    else
        force[activeTab].on("tick",node_tick );


    function node_tick() {
        node[activeTab].attr("transform", function (d) { return "translate(" + d.x + "," + d.y + ")"; });
    }

    function tick() {
        if (!drawnodesOnly) {
            link[activeTab].attr("x1", function (d) {
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


            node[activeTab].attr("transform", function (d) {
                return "translate(" + d.x + "," + d.y + ")";
            });

            edgepaths[activeTab].attr('d', function (d) {
                return 'M ' + d.source.x + ' ' + d.source.y + ' L ' + d.target.x + ' ' + d.target.y;
            });

            edgelabels[activeTab].attr('transform', function (d, i) {
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
        svg[activeTabIndex].attr("width",1500)
            .attr("height",1000);
        fullscreen=false;
    }
    else{
        $("#graphArea").removeClass("newSize").addClass("startSize");
        svg[activeTabIndex].attr("width",1050)
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
    var tabIndexToDownload;
    tabIndexToDownload=getActiveTabIndex();
    var addedSVGs=d3.selectAll("svg");
    var idName= "#"+addedSVGs[0][tabIndexToDownload].getAttribute("id");
    var graphDwn = d3.select("svg"+idName)
        .attr("version", 1.1)
        .attr("xmlns", "http://www.w3.org/2000/svg")
        .node().parentNode.innerHTML;


    var blob = new Blob([graphDwn], {type: "image/svg+xml"});
    saveAs(blob, "scholarlyNetwork.html");
    _LTracker.push({
        'method':'downloadGraphAsSVG',
        'text': 'Graph image downloaded',
        'Image-elements': graphDwn
    });
}


function downloadGraphAsData() {
    var tabIndexToDownload;
    tabIndexToDownload=getActiveTabIndex();
    var data=JSON.stringify(processedArray[tabIndexToDownload]);
    var blob_json = new Blob([data], { type: 'text/data;charset=utf-8;' });
    saveAs(blob_json,"data.json");
    _LTracker.push({
        'method':'downloadGraphAsData',
        'text': 'Graph data downloaded',
        'Data': data
    });
}

function clearSVG() {

    var tabIndexToCLear;
    tabIndexToCLear=getActiveTabIndex();
    var addedSVGs=d3.selectAll("svg");
    var idName= "#"+addedSVGs[0][tabIndexToCLear].getAttribute("id");
    $(idName).empty();
    //d3.selectAll("svg > *").remove();
    processedArray[tabIndexToCLear]=new Array();
    linksArray[tabIndexToCLear]=new Array();
    nodeAdded= new Set();
    authorAlreadyAdded[tabIndexToCLear]=new Map();
    publicationAlreadyAdded[tabIndexToCLear]=new Map();
    venueAlreadyAdded[tabIndexToCLear]=new Map();
    paperAlreadyAdded[tabIndexToCLear]=new Map()
    fosAlreadyAdded[tabIndexToCLear]=new Map();
    paperExpanded= new Set();
    nodeExpandedforRefernce = new Set();
    instituteAlreadyAdded[tabIndexToCLear]=new Map();
}

function update(activeTab) {
   var newHeight =30;
if(processedArray[activeTab].length>20) {
    d3.select("#paperGraphArea0 svg")
        .attr("width", width )
        .attr("height", height+500);
}
}

var vertexType={
    PAPER:"paper",
    AUTHOR:"author",
    ORG:"org",
    FOS:"fos",
    PUBLICATION:"journal",
    VENUE:"venue",
    CITES:"cites",
    REFERENCES:""
}
