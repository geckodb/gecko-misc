var tempArray=new Array();
var dataArray = new Array();
var linksArray = new Array();
var authorAlreadyAdded=new Map();
var paperAlreadyAdded=new Map();
var paperExpanded= new Set();
var nodeExpandedforRefernce = new Set();


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
                    }else{
                        var newLink = new createLinks(i, paperAlreadyAdded.get(tempArray[i].properties.inE[0].vertexProperties[j].PaperId));
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
