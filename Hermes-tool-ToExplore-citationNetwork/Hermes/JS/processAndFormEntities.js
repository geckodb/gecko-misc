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
    this.PaperId = paperId;
    this.properties = properties;
    this._source=new type("cite");
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

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);
    d3.selectAll("svg > *").remove();
    var mydata=new Set(processedArray);
    for(let item of mydata) console.log(item);
    createGraph(processedArray, linksArray,false);
    d3.select('.context-menu').style('display', 'none');
}


function showPublication(idToEXpand,processedArray) {

    var intial_length = processedArray.length;
    for (var i = 0; i < intial_length; i++) {
        if ((processedArray[i]._source.vType === vertexType.PAPER)&&(processedArray[i]._source.publisherPaper!==undefined)){
            if (processedArray[i]._id === idToEXpand) {
                    if (!publicationAlreadyAdded.has(processedArray[i]._source.publisherPaper)) {
                        var newNode = new createPublicationNode(processedArray[i]._source.publisherPaper)
                        var index = processedArray.push(newNode);
                        publicationAlreadyAdded.set(processedArray[i]._source.publisherPaper, index - 1);
                        var newLink = new createLinks(i, processedArray.length - 1);
                        linksArray.push(newLink);
                    }else{
                        var newLink = new createLinks(i, publicationAlreadyAdded.get(processedArray[i]._source.publisherPaper));
                        linksArray.push(newLink);
                    }
                }
            }
    }

    for (var i = 0; i < intial_length; i++) {
        if ((processedArray[i]._source.vType === vertexType.PAPER)&&(processedArray[i]._source.publisherPaper!==undefined)) {
            if (processedArray[i]._id !== idToEXpand) {
                    if (publicationAlreadyAdded.has(processedArray[i]._source.publisherPaper)) {
                        var newLink = new createLinks(i, publicationAlreadyAdded.get(processedArray[i]._source.publisherPaper));
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
    d3.select('.context-menu').style('display', 'none');
}


function showVenue(idToEXpand,processedArray) {

    var intial_length = processedArray.length;
    for (var i = 0; i < intial_length; i++) {
        if ((processedArray[i]._source.vType === vertexType.PAPER)&&(processedArray[i]._source.venuePaper!==undefined)){
            if (processedArray[i]._id === idToEXpand) {
                if (!venueAlreadyAdded.has(processedArray[i]._source.venue)) {
                    var newNode = new createVenueNode(processedArray[i]._source.venuePaper)
                    var index = processedArray.push(newNode);
                    venueAlreadyAdded.set(processedArray[i]._source.venue, index - 1);
                    var newLink = new createLinks(i, processedArray.length - 1);
                    linksArray.push(newLink);
                }else{
                    var newLink = new createLinks(i, venueAlreadyAdded.get(processedArray[i]._source.venuePaper));
                    linksArray.push(newLink);
                }
            }
        }
    }

    for (var i = 0; i < intial_length; i++) {
        if ((processedArray[i]._source.vType === vertexType.PAPER)&&(processedArray[i]._source.venue!==undefined)) {
            if (processedArray[i]._id !== idToEXpand) {
                if (venueAlreadyAdded.has(processedArray[i]._source.venuePaper)) {
                    var newLink = new createLinks(i, venueAlreadyAdded.get(processedArray[i]._source.venuePaper));
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
    d3.select('.context-menu').style('display', 'none');
}


function showCitations(idToEXpand) {
    var intial_length = processedArray.length;
    for (var i = 0; i < intial_length; i++) {
        if(processedArray[i].type===vertexType.PAPER){
            if((processedArray[i].PaperId===idToEXpand)&&(processedArray[i].properties.inE!==undefined)) {
                for (j = 0; j < processedArray[i].properties.inE[0].vertexProperties.length; j++) {
                    if (!paperAlreadyAdded.has(processedArray[i].properties.inE[0].vertexProperties[j].PaperId)) {
                        var newNode = new createPaperNode(processedArray[i].properties.inE[0].vertexProperties[j].PaperId, processedArray[i].properties.inE[0].vertexProperties[j].properties);
                        var index = processedArray.push(newNode);
                        var newLink = new createLinks(i, processedArray.length - 1);
                        paperAlreadyAdded.set(processedArray[i].properties.inE[0].vertexProperties[j].PaperId, index - 1);
                        linksArray.push(newLink);
                    }else{
                        var newLink = new createLinks(i, paperAlreadyAdded.get(processedArray[i].properties.inE[0].vertexProperties[j].PaperId));
                        linksArray.push(newLink);
                    }
                }
            }

        }

    }

    //creates links to existing nodes if new nodes have any relation with it
    for (var i = 0; i < intial_length; i++) {
        if (processedArray[i].type === vertexType.PAPER) {
            if ((processedArray[i].PaperId !== idToEXpand) && (processedArray[i].properties.inE !== undefined)) {
                for (var j = 0; j < processedArray[i].properties.inE[0].vertexProperties.length; j++) {
                    if (paperAlreadyAdded.has(processedArray[i].properties.inE[0].vertexProperties[j].PaperId)) {
                        var newLink = new createLinks(i, paperAlreadyAdded.get(processedArray[i].properties.inE[0].vertexProperties[j].PaperId));
                        linksArray.push(newLink);
                    }
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
    d3.select('.context-menu').style('display', 'none');
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


function showInstitution(idToEXpand,authorName,indexofCreatedAuthorNode) {
    var targetId="";
    var orgNames=new Array();
    var url= "http://localhost:9200/janusgraph_edgees/_search?q=eType=authorship AND srcId="+idToEXpand+" AND authorEdge="+authorName;

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
            var mydata=new Set(processedArray);
            for(let item of mydata) console.log(item);
            createGraph(processedArray, linksArray,false);

        });

    });
}


function showInstitutionFromAuthor(idToEXpand,processedArray) {
    //institute_paper=new Map();
    var intial_length = processedArray.length;

    for (var i = 0; i < intial_length; i++) {
        if ((processedArray[i]._source.vType === vertexType.AUTHOR)&&(processedArray[i]._source.orgList!==undefined)) {
            if ((processedArray[i]._id === idToEXpand)) {
                var newNode=new createInstitutionNode(processedArray[i]._source.orgList);
                var index=processedArray.push(newNode);
                var newLink=new createLinks(i,processedArray.length-1);
                linksArray.push(newLink);
                instituteAlreadyAdded.set(processedArray[i]._source.orgList,index-1);
                //institute_paper.set(processedArray[i]._source.orgList);
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



function showFOS(idToEXpand,processedArray) {

    var intial_length = processedArray.length;
    for (var i = 0; i < intial_length; i++) {
        if (processedArray[i]._source.vType === vertexType.PAPER) {
            if ((processedArray[i]._id === idToEXpand) && (processedArray[i]._source.fosPaper!== undefined)) {
                for (var j = 0; j < processedArray[i]._source.fosPaper.length; j++) {
                    if (!fosAlreadyAdded.has(processedArray[i]._source.fosPaper[j])) {
                        var newNode = new createFOSNode(processedArray[i]._id, processedArray[i]._source.fosPaper[j])
                        var index = processedArray.push(newNode);
                        fosAlreadyAdded.set(processedArray[i]._source.fosPaper[j], index - 1);
                        var newLink = new createLinks(i, processedArray.length - 1);
                        linksArray.push(newLink);
                    }else{
                        var newLink = new createLinks(i, fosAlreadyAdded.get(processedArray[i]._source.fosPaper[j]));
                        linksArray.push(newLink);
                    }
                }
            }
        }
    }

    for (var i = 0; i < intial_length; i++) {
        if (processedArray[i]._source.vType === vertexType.PAPER) {
            if ((processedArray[i]._id !== idToEXpand) && (!paperExpanded.has(processedArray[i]._id)) && (processedArray[i]._source.fosPaper !== undefined)) {
                for (var j = 0; j < processedArray[i]._source.fosPaper.length; j++) {
                    if (fosAlreadyAdded.has(processedArray[i]._source.fosPaper[j].name)) {
                        var newLink = new createLinks(i, fosAlreadyAdded.get(processedArray[i]._source.fosPaper[j].name));
                        linksArray.push(newLink);
                    }
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
    d3.select('.context-menu').style('display', 'none');
}