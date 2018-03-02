var linksArray = new Array();
var authorAlreadyAdded=new Map();
var paperAlreadyAdded=new Map();
var paperExpanded= new Set();
var nodeExpandedforRefernce = new Set();
var institute=new Map();

function createAuthorNode(paperId, authorName) {
    this.authorId = paperId;
    this.authors = authorName;
    this._source.vType="author";
}

function createPaperNode(paperId, properties) {
    this.PaperId = paperId;
    this.properties = properties;
    this._source.vType="cite";
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

function createInstitutionNode(name) {
    this.name = name;
    this.type="institution";
}

function showAuthors(idToEXpand,processedArray) {

    var intial_length = processedArray.length;
    for (var i = 0; i < intial_length; i++) {
        if (processedArray[i]._source.vType === "paper") {
            if ((processedArray[i]._id === idToEXpand) && (processedArray[i]._source.authors !== undefined)) {
                for (var j = 0; j < processedArray[i]._source.authors.length; j++) {
                    if (!authorAlreadyAdded.has(processedArray[i]._source.authors[j])) {
                        var newNode = new createAuthorNode(processedArray[i]._id, processedArray[i]._source.authors[j].name)
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
        if (processedArray[i].type === "paper") {
            if ((processedArray[i].PaperId !== idToEXpand) && (!paperExpanded.has(processedArray[i].PaperId)) && (processedArray[i].properties !== undefined)) {
                for (var j = 0; j < processedArray[i].properties.authors.length; j++) {
                    if (authorAlreadyAdded.has(processedArray[i].properties.authors[j].name)) {
                        var newLink = new createLinks(i, authorAlreadyAdded.get(processedArray[i].properties.authors[j].name));
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

function showCitations(idToEXpand) {
    var intial_length = processedArray.length;
    for (var i = 0; i < intial_length; i++) {
        if(processedArray[i].type==="paper"){
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
        if (processedArray[i].type === "paper") {
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


function showInstitution(idToEXpand) {
    institute_paper=new Map();
    var intial_length = processedArray.length;
    for (var i = 0; i < intial_length; i++) {
        if (processedArray[i].type === "paper") {
            if ((processedArray[i].PaperId === idToEXpand) && (processedArray[i].properties.authors !== undefined)) {
                for (var j = 0; j < processedArray[i].properties.authors.length; j++) {
                    if((!institute_paper.has(processedArray[i].properties.authors[j].org))&&institute.has(processedArray[i].properties.authors[j].org)){
                        var newLink=new createLinks(i,institute.get(processedArray[i].properties.authors[j].org));
                        linksArray.push(newLink);
                        institute_paper.set(processedArray[i].properties.authors[j].org);
                    }
                    if(!institute_paper.has(processedArray[i].properties.authors[j].org)){
                       var newNode=new createInstitutionNode(processedArray[i].properties.authors[j].org);
                        var index=processedArray.push(newNode);
                       var newLink=new createLinks(i,processedArray.length-1);
                       linksArray.push(newLink);
                       institute.set(processedArray[i].properties.authors[j].org,index-1);
                       institute_paper.set(processedArray[i].properties.authors[j].org);
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