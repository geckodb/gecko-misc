var linksArray = new Array();
var authorAlreadyAdded=new Map();
var publicationAlreadyAdded=new Map();
var venueAlreadyAdded=new Map();
var fosAlreadyAdded=new Map();
var paperAlreadyAdded=new Map();
var paperExpanded= new Set();
var nodeExpandedforRefernce = new Set();
var instituteAlreadyAdded=new Map();


class _source {
    constructor(state, value) {
        switch (state) {
            case "author":
                this.vType = state;
                this.author = value;
                break;
            case "org":
                this.vType = state;
                this.org = value;
                break;
            case "fos":
                this.vType = state;
                this.fos = value;
                break;
            case "publication":
                this.vType = state;
                this.publisher = value;
                break;
            case "venue":
                this.vType = state;
                this.venue = value;
                break;
            case "cites":
                this.vType=state;
                this.title=value._source.titleCited;
                this.authors=value._source.authorsCited;

        }
    }
}

function createAuthorNode(paperId, author) {
    this.createdNode=true;
    this.authorId = paperId;
    this._source=new _source("author",author);
 }

function createFOSNode(paperId, fos) {
    this.id = paperId;
    this._source=new _source("fos",fos);
}

function createPaperNode(paperId, properties) {
    this._id = paperId;
    this._source=new _source("cites",properties);
}

function createLinks(source, target) {
    this.source = source;
    this.target = target;
}

function createRefernceNode(paperId, properties) {
    this.PaperId = paperId;
    this.properties = properties;
    this._source=new type("reference");
}

function createInstitutionNode(name) {
    this.name = name;
    this._source=new _source("org",name);
}

function createVenueNode(name) {
    this.name = name;
    this._source=new _source("venue",name);
}

function createPublicationNode(name) {
    this.name = name;
    this._source=new _source("publication",name);
}


function showAuthors(idToEXpand, paperjgId,processedArray) {
    $("#graphArea").css("cursor","wait");
    var intial_length = processedArray.length;
    for (var i = 0; i < intial_length; i++) {
        if (processedArray[i]._source.vType === "paper") {
            if ((processedArray[i]._id === idToEXpand) && (processedArray[i]._source.authors!== undefined)) {
                for (var j = 0; j < processedArray[i]._source.authors.length; j++) {
                    if (!authorAlreadyAdded.has(processedArray[i]._source.authors[j])) {
                        var newNode = new createAuthorNode(paperjgId, processedArray[i]._source.authors[j])
                        var index = processedArray.push(newNode);
                        authorAlreadyAdded.set(processedArray[i]._source.authors[j], index - 1);
                        var newLink = new createLinks(i, processedArray.length - 1);
                        linksArray.push(newLink);
                    }else{
                        var newLink = new createLinks(i, authorAlreadyAdded.get(processedArray[i]._source.authors[j]));
                        linksArray.push(newLink);
                    }
                }
            }
        }
    }

    for (var i = 0; i < intial_length; i++) {
        if (processedArray[i]._source.vType === vertexType.PAPER) {
            if ((processedArray[i]._id !== idToEXpand) && (!paperExpanded.has(processedArray[i]._id)) && (processedArray[i]._source.authors !== undefined)) {
                for (var j = 0; j < processedArray[i]._source.authors.length; j++) {
                    if (authorAlreadyAdded.has(processedArray[i]._source.authors[j].name)) {
                        var newLink = new createLinks(i, authorAlreadyAdded.get(processedArray[i]._source.authors[j].name));
                        linksArray.push(newLink);
                    }
                }
            }
        }
    }
    for (var i = 0; i < intial_length; i++) {
        if (processedArray[i]._source.vType === "cites") {
            if ((processedArray[i]._id === idToEXpand)) {
                for (var j = 0; j < processedArray[i]._source.authors.length; j++) {
                    var newNode = new createAuthorNode(paperjgId, processedArray[i]._source.authors[j])
                    var index = processedArray.push(newNode);
                    authorAlreadyAdded.set(processedArray[i]._source.authors[j], index - 1);
                    var newLink = new createLinks(i, processedArray.length - 1);
                    linksArray.push(newLink);
                }
            }
        }
    }

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);
    d3.selectAll("svg > *").remove();
    var mydata=new Set(processedArray);
    for(let item of mydata) console.log(item);
    createGraph(processedArray, linksArray,false);
    $("#graphArea").css("cursor","default");
    d3.select('.context-menu').style('display', 'none');
}


function showPublication(idToEXpand,selectedIndex,processedArray) {
    $("#graphArea").css("cursor","wait");

    if(processedArray[selectedIndex]._source.publisherPaper===undefined){
        alert("Publication data not available for selected paper");
    }

        if ((processedArray[selectedIndex]._source.vType === vertexType.PAPER)&&(processedArray[selectedIndex]._source.publisherPaper!==undefined)){
            if (processedArray[selectedIndex]._id === idToEXpand) {
                    if (!publicationAlreadyAdded.has(processedArray[selectedIndex]._source.publisherPaper)) {
                        var newNode = new createPublicationNode(processedArray[selectedIndex]._source.publisherPaper)
                        var index = processedArray.push(newNode);
                        publicationAlreadyAdded.set(processedArray[selectedIndex]._source.publisherPaper, index - 1);
                        var newLink = new createLinks(selectedIndex, processedArray.length - 1);
                        linksArray.push(newLink);
                    }else{
                        var newLink = new createLinks(selectedIndex, publicationAlreadyAdded.get(processedArray[selectedIndex]._source.publisherPaper));
                        linksArray.push(newLink);
                    }
                }
            }

        if ((processedArray[selectedIndex]._source.vType === vertexType.PAPER)&&(processedArray[selectedIndex]._source.publisherPaper!==undefined)) {
            if (processedArray[selectedIndex]._id !== idToEXpand) {
                    if (publicationAlreadyAdded.has(processedArray[selectedIndex]._source.publisherPaper)) {
                        var newLink = new createLinks(selectedIndex, publicationAlreadyAdded.get(processedArray[selectedIndex]._source.publisherPaper));
                        linksArray.push(newLink);
                    }
            }
        }

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);
    d3.selectAll("svg > *").remove();
    var mydata=new Set(processedArray);
    for(let item of mydata) console.log(item);
    createGraph(processedArray, linksArray,false);
    $("#graphArea").css("cursor","default");
    d3.select('.context-menu').style('display', 'none');
}


function showVenue(idToEXpand,selectedIndex,processedArray) {
    $("#graphArea").css("cursor","wait");

    if(processedArray[selectedIndex]._source.venuePaper===undefined){
        alert("Publication data not available for selected paper");
    }

        if ((processedArray[selectedIndex]._source.vType === vertexType.PAPER)&&(processedArray[selectedIndex]._source.venuePaper!==undefined)){
            if (processedArray[selectedIndex]._id === idToEXpand) {
                if (!venueAlreadyAdded.has(processedArray[selectedIndex]._source.venuePaper)) {
                    var newNode = new createVenueNode(processedArray[selectedIndex]._source.venuePaper);
                    var index = processedArray.push(newNode);
                    venueAlreadyAdded.set(processedArray[selectedIndex]._source.venuePaper, index - 1);
                    var newLink = new createLinks(selectedIndex, processedArray.length - 1);
                    linksArray.push(newLink);
                }else{
                    var newLink = new createLinks(selectedIndex, venueAlreadyAdded.get(processedArray[selectedIndex]._source.venuePaper));
                    linksArray.push(newLink);
                }
            }
        }

        if ((processedArray[selectedIndex]._source.vType === vertexType.PAPER)&&(processedArray[selectedIndex]._source.venuePaper!==undefined)) {
            if (processedArray[selectedIndex]._id !== idToEXpand) {
                if (venueAlreadyAdded.has(processedArray[selectedIndex]._source.venuePaper)) {
                    var newLink = new createLinks(selectedIndex, venueAlreadyAdded.get(processedArray[selectedIndex]._source.venuePaper));
                    linksArray.push(newLink);
                }
            }
        }

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);
    d3.selectAll("svg > *").remove();
    var mydata=new Set(processedArray);
    for(let item of mydata) console.log(item);
    createGraph(processedArray, linksArray,false);
    $("#graphArea").css("cursor","default");
    d3.select('.context-menu').style('display', 'none');
}


function showCitations(idToEXpand,janusGraphId,scrIdPaperIndex,processedArray) {
    var urlCitation="http://localhost:9200/janusgraph_edgees/_search?q=srcId:"+janusGraphId;
    d3.json(urlCitation, function (error, json) {
        if (error) throw error;
        var tempArray= new Array();
        tempArray=json.hits.hits;
        if(json.hits.hits.length===0){
            alert("Cited by relation data not available for the selected paper")
        }
        for (var i=0;i<tempArray.length;i++) {
                var newNode = new createPaperNode(tempArray[i]._source.tgtId, tempArray[i]);
                var index = processedArray.push(newNode);
                var newLink = new createLinks(scrIdPaperIndex, processedArray.length - 1);
                //paperAlreadyAdded.set(processedArray[i].properties.inE[0].vertexProperties[j].PaperId, index - 1);
                linksArray.push(newLink);

                JSON.stringify(linksArray);
                JSON.stringify(processedArray);
                d3.selectAll("svg > *").remove();
                var mydata = new Set(processedArray);
                for (let item of mydata) console.log(item);
                createGraph(processedArray, linksArray, false);
                d3.select('.context-menu').style('display', 'none');
            }
    });
  }

function showReferences(idToEXpand){
    var intial_length = processedArray.length;
    for (var i = 0; i < intial_length; i++) {
        if((processedArray[i].PaperId===idToEXpand) && (!nodeExpandedforRefernce.has(idToEXpand))){
            for(j=0;j<processedArray[i].properties.outE[0].vertexProperties.length;j++){
                var newNode = new createRefernceNode(processedArray[i].properties.outE[0].vertexProperties[j].PaperId, processedArray[i].properties.outE[0].vertexProperties[j].properties);
                var index= processedArray.push(newNode);
                var newLink = new createLinks(i,processedArray.length - 1);
                linksArray.push(newLink);
            }

        }
    }
    JSON.stringify(linksArray);
    JSON.stringify(processedArray);
    d3.selectAll("svg > *").remove();
    var mydata=new Set(processedArray);
    for(let item of mydata) console.log(item);
    createGraph(processedArray, linksArray,false);
    d3.select('.context-menu').style('display', 'none');
}


function showInstitution(idToEXpand,authorName,indexofCreatedAuthorNode,processedArray) {
    var targetId="";
    var orgNames=new Array();
    var url= "http://localhost:9200/janusgraph_edgees/_search?q=eType=authorship AND srcId="+idToEXpand+" AND authorEdge="+authorName;
    $("#graphArea").css("cursor","wait");
    d3.json(url, function (error, json) {
         if (error) throw error;
         console.log(json)
         targetId = json.hits.hits[0]._source.tgtId;
        var getAuthorData="http://localhost:9200/janusgraph_vertexes/_search?q=vType:author AND jgId:"+targetId;

        d3.json(getAuthorData,function (error, jsonResult) {
            if (error) throw error;
            orgNames = jsonResult.hits.hits[0]._source.orgList;


            if((orgNames!=undefined)){
                var newNode=new createInstitutionNode(orgNames);
                var index=processedArray.push(newNode);
                var newLink=new createLinks(indexofCreatedAuthorNode,processedArray.length-1);
                linksArray.push(newLink);
                instituteAlreadyAdded.set(orgNames);
            }else{
                alert("Membership data not available ")
            }

            JSON.stringify(linksArray);
            JSON.stringify(processedArray);
            d3.selectAll("svg > *").remove();
            createGraph(processedArray, linksArray,false);
            $("#graphArea").css("cursor","default");
        });

    });

}


function showInstitutionFromAuthor(idToEXpand,selectedIndex,processedArray) {

    $("#graphArea").css("cursor","wait");
    if(processedArray[selectedIndex]._source.orgList===undefined){
        alert("Institution information not available")
    }

        if ((processedArray[selectedIndex]._source.vType === vertexType.AUTHOR)&&(processedArray[selectedIndex]._source.orgList!==undefined)) {
            if ((processedArray[selectedIndex]._id === idToEXpand)) {
                var newNode=new createInstitutionNode(processedArray[selectedIndex]._source.orgList);
                var index=processedArray.push(newNode);
                var newLink=new createLinks(selectedIndex,processedArray.length-1);
                linksArray.push(newLink);
                instituteAlreadyAdded.set(processedArray[selectedIndex]._source.orgList,index-1);
            }
        }
   // }

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);
    d3.selectAll("svg > *").remove();
    createGraph(processedArray, linksArray,false);
    $("#graphArea").css("cursor","default");
    d3.select('.context-menu').style('display', 'none');
}



function showFOS(idToEXpand,selectedIndex,processedArray) {
    $("#graphArea").css("cursor","wait");
   // var intial_length = processedArray.length;
    //for (var i = 0; i < intial_length; i++) {
    if(processedArray[selectedIndex]._source.fosPaper=== undefined){
        alert("Domain information for the selected paper is not available");
    }

        if (processedArray[selectedIndex]._source.vType === vertexType.PAPER) {
            if ((processedArray[selectedIndex]._id === idToEXpand) && (processedArray[selectedIndex]._source.fosPaper!== undefined)) {
                for (var j = 0; j < processedArray[selectedIndex]._source.fosPaper.length; j++) {
                    if (!fosAlreadyAdded.has(processedArray[selectedIndex]._source.fosPaper[j])) {
                        var newNode = new createFOSNode(processedArray[selectedIndex]._id, processedArray[selectedIndex]._source.fosPaper[j])
                        var index = processedArray.push(newNode);
                        fosAlreadyAdded.set(processedArray[selectedIndex]._source.fosPaper[j], index - 1);
                        var newLink = new createLinks(selectedIndex, processedArray.length - 1);
                        linksArray.push(newLink);
                    }else{
                        var newLink = new createLinks(selectedIndex, fosAlreadyAdded.get(processedArray[selectedIndex]._source.fosPaper[j]));
                        linksArray.push(newLink);
                    }
                }
            }
        }

        if (processedArray[selectedIndex]._source.vType === vertexType.PAPER) {
            if ((processedArray[selectedIndex]._id !== idToEXpand) && (!paperExpanded.has(processedArray[selectedIndex]._id)) && (processedArray[selectedIndex]._source.fosPaper !== undefined)) {
                for (var j = 0; j < processedArray[selectedIndex]._source.fosPaper.length; j++) {
                    if (fosAlreadyAdded.has(processedArray[selectedIndex]._source.fosPaper[j].name)) {
                        var newLink = new createLinks(selectedIndex, fosAlreadyAdded.get(processedArray[selectedIndex]._source.fosPaper[j].name));
                        linksArray.push(newLink);
                    }
                }
            }
        }

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);
    d3.selectAll("svg > *").remove();
    var mydata=new Set(processedArray);
    for(let item of mydata) console.log(item);
    createGraph(processedArray, linksArray,false);
    $("#graphArea").css("cursor","default");
    d3.select('.context-menu').style('display', 'none');
}

function showCompleteDetails(idsOfDetailToShow,processedArray){
    $('#modalcontents').html('');
    var modalbody=document.getElementById("modalcontents");

    //To bind Title
    var tr_title = document.createElement("tr");
    var td_titleKey = document.createElement("td");
    td_titleKey.setAttribute("id","tdHeader");
    var td_titleVal=document.createElement("td");
    td_titleVal.setAttribute("id","tdValue");
    titleHeader_b = document.createElement("b");
    titleHeader_b.innerText="Title : ";
    td_titleKey.appendChild(titleHeader_b);
    tr_title.appendChild(td_titleKey);
    titleVal_p=document.createElement("h5");
    titleVal_p.innerText=processedArray[idsOfDetailToShow]._source.title;
    td_titleVal.appendChild(titleVal_p);
    tr_title.appendChild(td_titleVal);
    modalbody.appendChild(tr_title);

    //To bind year of publish
    if(processedArray[idsOfDetailToShow]._source.year!==undefined) {
        var tr_year = document.createElement("tr");
        var td_yearKey = document.createElement("td");
        td_yearKey.setAttribute("id", "tdHeader");
        var td_yearVal = document.createElement("td");
        td_yearVal.setAttribute("id","tdValue");
        yearHeader_b = document.createElement("b");
        yearHeader_b.innerText = "Year : ";
        td_yearKey.appendChild(yearHeader_b);
        tr_year.appendChild(td_yearKey);
        yearVal_p = document.createElement("h5");
        yearVal_p.innerText = processedArray[idsOfDetailToShow]._source.year;
        td_yearVal.appendChild(yearVal_p);
        tr_year.appendChild(td_yearVal);

        modalbody.appendChild(tr_year);
    }

    //To bind abstract
    if(processedArray[idsOfDetailToShow]._source.abstract!==undefined){
        var tr_abstract = document.createElement("tr");
        var td_abstractKey = document.createElement("td");
        td_abstractKey.setAttribute("id","tdHeader");
        var td_abstractVal=document.createElement("td");
        td_abstractVal.setAttribute("id","tdValue");
        abstractHeader_b = document.createElement("b");
        abstractHeader_b.innerText="Abstract : ";
        td_abstractKey.appendChild(abstractHeader_b);
        tr_abstract.appendChild(td_abstractKey);
        abstractVal_p=document.createElement("h5");
        abstractVal_p.innerText=processedArray[idsOfDetailToShow]._source.abstract;
        td_abstractVal.appendChild(abstractVal_p);
        tr_abstract.appendChild(td_abstractVal);
        modalbody.appendChild(tr_abstract);
    }

    //To bind Authors
    if(processedArray[idsOfDetailToShow]._source.authors!==undefined) {
        var tr_authors = document.createElement("tr");
        var td_authorKey = document.createElement("td");
        td_authorKey.setAttribute("id", "tdHeader");
        var td_authorVal = document.createElement("td");
        td_authorVal.setAttribute("id","tdValue");
        authorHeader = document.createElement("b");
        authorHeader.innerText = "Authors : ";
        td_authorKey.appendChild(authorHeader);
        tr_authors.appendChild(td_authorKey);
        authorVal = document.createElement("h5");
        authorVal.innerText = processedArray[idsOfDetailToShow]._source.authors;
        td_authorVal.appendChild(authorVal);
        tr_authors.appendChild(td_authorVal);
        modalbody.appendChild(tr_authors);
    }
    //To bind links
    if(processedArray[idsOfDetailToShow]._source.url!==undefined) {
        var tr_url = document.createElement("tr");
        urlHeader = document.createElement("b");
        urlHeader.innerText = "Links : ";
        var td_urlKey = document.createElement("td");
        td_urlKey.setAttribute("id", "tdHeader");
        td_urlKey.appendChild(urlHeader);
        tr_url.appendChild(td_urlKey);
        var td_urlVal = document.createElement("td");
        td_urlVal.setAttribute("id","tdValue");
        for (var i = 0; i < processedArray[idsOfDetailToShow]._source.url.length; i++) {
            a = document.createElement("a");
            a.innerText = processedArray[idsOfDetailToShow]._source.url[i];
            a.setAttribute("href", processedArray[idsOfDetailToShow]._source.url[i]);
            a.setAttribute("target", "_blank");
            td_urlVal.appendChild(a);
            if (i < processedArray[idsOfDetailToShow]._source.url.length) {
                linebr = document.createElement("br");
                td_urlVal.appendChild(linebr);
            }
        }
        tr_url.appendChild(td_urlVal);
        modalbody.appendChild(tr_url);

    }

    //To bind FOS
    if(processedArray[idsOfDetailToShow]._source.fosPaper!==undefined) {
        var tr_FOS = document.createElement("tr");
        var td_FOSKey = document.createElement("td");
        td_FOSKey.setAttribute("id", "tdHeader");
        var td_FOSVal = document.createElement("td");
        td_FOSVal.setAttribute("id","tdValue");
        FOSHeader_b = document.createElement("b");
        FOSHeader_b.innerText = "Field of study : ";
        td_FOSKey.appendChild(FOSHeader_b);
        tr_FOS.appendChild(td_FOSKey);
        FOSVal_p = document.createElement("h5");
        FOSVal_p.innerText = processedArray[idsOfDetailToShow]._source.fosPaper;
        td_FOSVal.appendChild(FOSVal_p);
        tr_FOS.appendChild(td_FOSVal);
        modalbody.appendChild(tr_FOS);
    }
    //To bind Venue
    if(processedArray[idsOfDetailToShow]._source.venue!==undefined) {
        var tr_venue = document.createElement("tr");
        var td_venueKey = document.createElement("td");
        td_venueKey.setAttribute("id", "tdHeader");
        var td_venueVal = document.createElement("td");
        td_venueVal.setAttribute("id","tdValue");
        venueHeader_b = document.createElement("b");
        venueHeader_b.innerText = "Venue : ";
        td_venueKey.appendChild(venueHeader_b);
        tr_venue.appendChild(td_venueKey);
        venueVal_p = document.createElement("h5");
        venueVal_p.innerText = processedArray[idsOfDetailToShow]._source.venuePaper;
        td_venueVal.appendChild(venueVal_p);
        tr_venue.appendChild(td_venueVal);
        modalbody.appendChild(tr_venue);
    }
    //To bind publisher
    if(processedArray[idsOfDetailToShow]._source.publisherPaper!==undefined) {
        var tr_publisher = document.createElement("tr");
        var td_publisherKey = document.createElement("td");
        td_publisherKey.setAttribute("id", "tdHeader");
        var td_publisherVal = document.createElement("td");
        td_publisherVal.setAttribute("id","tdValue");
        publisherHeader_b = document.createElement("b");
        publisherHeader_b.innerText = "Publisher : ";
        td_publisherKey.appendChild(publisherHeader_b);
        tr_publisher.appendChild(td_publisherKey);
        publisherVal_p = document.createElement("h5");
        publisherVal_p.innerText = processedArray[idsOfDetailToShow]._source.publisherPaper;
        td_publisherVal.appendChild(publisherVal_p);
        tr_publisher.appendChild(td_publisherVal);
        modalbody.appendChild(tr_publisher);
    }
    $("#myModal").modal();
}

function addTag() {
    $('#modalInput').html('');
    $("#addTagModal").modal();

}

function CloseAddTagWindow() {
    $("#addTagModal").modal('hide');
}