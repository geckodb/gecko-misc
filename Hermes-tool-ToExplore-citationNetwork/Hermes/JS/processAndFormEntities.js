var linksArray = [[]];
var authorAlreadyAdded=[[]];
var publicationAlreadyAdded=[[]];
var venueAlreadyAdded=[[]];
var coAuthorsAlreadyAdded=[[]];
var fosAlreadyAdded=[[]];
var paperAlreadyAdded=[[]];
var paperExpanded= new Set();
var nodeExpandedforRefernce = new Set();
var instituteAlreadyAdded=[[]];
var addedSVGs;
var idName;

authorAlreadyAdded[0]=new Map();
publicationAlreadyAdded[0]=new Map();
venueAlreadyAdded[0]=new Map();
coAuthorsAlreadyAdded[0]=new Map();
fosAlreadyAdded[0]=new Map();
instituteAlreadyAdded[0]=new Map();
paperAlreadyAdded[0]=new Map();

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
            case "journal":
                this.vType = state;
                this.publisher = value;
                break;
            case "venue":
                this.vType = state;
                this.venue = value;
                break;
            case "cites":
                this.vType=state;
                this.title=value._source.titleCiter;
            //this.authors=value._source.authorsCited;

        }
    }
}

function createAuthorNode(authorId, author) {
    this.createdNode=true;
    this.authorId = authorId;
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

function createLinks(source, target,edgeName) {
    this.source = source;
    this.target = target;
    this.label=edgeName;
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

function showAuthors(idToEXpand,index, processedArray,activeTab) {
    var authorIds=[];
    var authorcompltDetails=[];
    var auids;

    $("#graphArea").css("cursor","wait");
    var intial_length = processedArray.length;
    if (processedArray[index]._source.vType === "paper") {
        if (processedArray[index]._source.PAPER_ID === idToEXpand) {
            _LTracker.push({
                'method':'showAuthors',
                'tag': 'Authors',
                'value':processedArray[index]
            });

            var urlforAuthorId="http://localhost:9200/dblprelation_authorship/_search?q=PAPER_ID:"+"\""+idToEXpand+"\"";
            d3.json(urlforAuthorId,function (error,resultIds) {
                if (error) throw error;

                authorIds=resultIds.hits.hits;
                if(authorIds.length>0){
                    //http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:("au_53526" OR "au_195445" OR "au_145781" OR "au_93313")
                    for(var m=0;m<authorIds.length;m++){

                        if( m<authorIds.length-1){
                            auids=auids+("\""+authorIds[m]._source.AUTHOR_ID+"\"")+"OR";
                        }
                        else{
                            auids=auids+("\""+authorIds[m]._source.AUTHOR_ID+"\"");
                        }
                    }
                }
                var urlforAuthorName="http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:("+auids.substring(9,auids.length)+")";
                d3.json(urlforAuthorName,function (error,resultnames) {
                    if (error) throw error;
                    authorcompltDetails=resultnames.hits.hits;

                    for (var j = 0; j < authorcompltDetails.length; j++) {
                        if (!authorAlreadyAdded[activeTab].has(authorcompltDetails[j]._source.AUTHOR_NAME)) {
                            //  var newNode = new createAuthorNode(paperjgId, processedArray[i]._source.authors[j])
                            var idPos = processedArray.push(authorcompltDetails[j]);
                            authorAlreadyAdded[activeTab].set(authorcompltDetails[j]._source.AUTHOR_NAME, idPos - 1);
                            var newLink = new createLinks(index, processedArray.length - 1,"has_author");
                            linksArray[activeTab].push(newLink);
                        }else{
                            var newLink = new createLinks(index, authorAlreadyAdded[activeTab].get(authorcompltDetails[j]._source.AUTHOR_NAME),"has_author");
                            linksArray[activeTab].push(newLink);
                        }
                    }

                    /* for (var i = 0; i < intial_length; i++) {
                         if (processedArray[i]._source.vType === vertexType.PAPER) {
                             if (processedArray[i]._source.PAPER_ID !== idToEXpand) {
                                 var authorsofOthepaper=processedArray[i]._source.Authors.split(";");
                                 for (var j = 0; j <authorsofOthepaper.length; j++) {
                                     if (authorAlreadyAdded[activeTab].has(authorsofOthepaper[j])) {
                                         var newLink = new createLinks(i, authorAlreadyAdded[activeTab].get(authorsofOthepaper[j]));
                                         linksArray[activeTab].push(newLink);
                                     }
                                 }
                             }
                         }
                     }*/


                    JSON.stringify(linksArray);
                    JSON.stringify(processedArray);
                    addedSVGs=d3.selectAll("svg");
                    idName= "#"+addedSVGs[0][activeTab].getAttribute("id");
                    $(idName).empty();

                    createGraph(processedArray, linksArray[activeTab],false,false,activeTab);
                    $("#graphArea").css("cursor","default");
                    d3.select('.context-menu').style('display', 'none');

                });
            });


        }
    }
}

function showJournals(index, processedArray,activeTab) {
    var journalIds = [];
    var journalcompltDetails = [];
    var jouids;


    $("#graphArea").css("cursor", "wait");
    var intial_length = processedArray.length;
    if (processedArray[index]._source.vType === vertexType.PAPER) {
        _LTracker.push({
            'method': 'showJournals',
            'tag': 'Journals',
            'value': processedArray[index]
        });

        var urlforjournalId = "http://localhost:9200/dblprelation_journalofpaper/_search?q=PAPER_ID:" + "\"" + processedArray[index]._source.PAPER_ID + "\"";
        d3.json(urlforjournalId, function (error, resultIds) {
            if (error) throw error;
            journalIds=resultIds.hits.hits;
            if(journalIds.length>0){
                //http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:("au_53526" OR "au_195445" OR "au_145781" OR "au_93313")
                for(var m=0;m<journalIds.length;m++){

                    if( m<journalIds.length-1){
                        jouids=jouids+("\""+journalIds[m]._source.JOURNAL_ID+"\"")+"OR";
                    }
                    else{
                        jouids=jouids+("\""+journalIds[m]._source.JOURNAL_ID+"\"");
                    }
                }
            }
            if(jouids!==undefined) {
                var urlforJournalName = "http://localhost:9200/dblpvertexes/_search?q=vType:journal AND JOURNAL_ID:(" + jouids.substring(9, jouids.length) + ")";
                d3.json(urlforJournalName, function (error, resultnames) {
                    if (error) throw error;
                    journalcompltDetails = resultnames.hits.hits;

                    for (var j = 0; j < journalcompltDetails.length; j++) {
                        if (!publicationAlreadyAdded[activeTab].has(journalcompltDetails[j]._source.JOURNAL_NAME)) {

                            var idPos = processedArray.push(journalcompltDetails[j]);
                            publicationAlreadyAdded[activeTab].set(journalcompltDetails[j]._source.JOURNAL_NAME, idPos - 1);
                            var newLink = new createLinks(index, processedArray.length - 1, "has_journal");
                            linksArray[activeTab].push(newLink);
                        } else {
                            var newLink = new createLinks(index, publicationAlreadyAdded[activeTab].get(journalcompltDetails[j]._source.JOURNAL_NAME), "has_journal");
                            linksArray[activeTab].push(newLink);
                        }
                    }

                    JSON.stringify(linksArray);
                    JSON.stringify(processedArray);
                    addedSVGs = d3.selectAll("svg");
                    idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
                    $(idName).empty();

                    createGraph(processedArray, linksArray[activeTab], false, false, activeTab);
                    $("#graphArea").css("cursor", "default");
                    d3.select('.context-menu').style('display', 'none');

                });
            }else{
                alert("Publication of the paper is not available");
                $("#graphArea").css("cursor", "default");
                d3.select('.context-menu').style('display', 'none');
            }
        });
    }
}

function showPapersPublished(index,processedArray,activeTab) {
    var paperIds=[];
    var papercompltDetails=[];
    var pids;
    $("#graphArea").css("cursor","wait");

    var intial_length = processedArray.length;
    if (processedArray[index]._source.vType === "journal") {
        //  if (processedArray[index]._source.AUTHOR_ID === authorId) {
        _LTracker.push({
            'method':'showPapersPublished',
            'tag': 'Journals',
            'value':processedArray[index]
        });

        var urlforpaperId="http://localhost:9200/dblprelation_journalofpaper/_search?q=JOURNAL_ID:"+"\""+processedArray[index]._source.JOURNAL_ID+"\""+"&size=90";
        d3.json(urlforpaperId,function (error,resultIds) {
            if (error) throw error;

            paperIds=resultIds.hits.hits;
            if(paperIds.length>1){
                for(var m=0;m<paperIds.length;m++){

                    if( m<paperIds.length-1){
                        pids=pids+("\""+paperIds[m]._source.PAPER_ID+"\"")+"OR";
                    }
                    else{
                        pids=pids+("\""+paperIds[m]._source.PAPER_ID+"\"");
                    }
                }
            }else if(paperIds.length===1){
                pids=pids+("\""+paperIds[0]._source.PAPER_ID+"\"");
            }

            var urlforPapers="http://localhost:9200/dblpvertexes/_search?q=vType:paper AND PAPER_ID:("+pids.substring(9,pids.length)+")&size=90";
            d3.json(urlforPapers,function (error,resultnames) {
                if (error) throw error;
                papercompltDetails=resultnames.hits.hits;

                for (var j = 0; j < papercompltDetails.length; j++) {
                    if (!paperAlreadyAdded[activeTab].has(papercompltDetails[j]._source.PAPER_ID)) {

                        var idPos = processedArray.push(papercompltDetails[j]);
                        paperAlreadyAdded[activeTab].set(papercompltDetails[j]._source.PAPER_ID, idPos - 1);
                        var newLink = new createLinks(index, processedArray.length - 1,"has_paper");
                        linksArray[activeTab].push(newLink);
                    }else{
                        var newLink = new createLinks(index, paperAlreadyAdded[activeTab].get(papercompltDetails[j]._source.PAPER_ID),"has_paper");
                        linksArray[activeTab].push(newLink);
                    }
                }
                JSON.stringify(linksArray);
                JSON.stringify(processedArray);
                addedSVGs=d3.selectAll("svg");
                idName= "#"+addedSVGs[0][activeTab].getAttribute("id");
                $(idName).empty();

                createGraph(processedArray, linksArray[activeTab],false,false,activeTab);
                $("#graphArea").css("cursor","default");
                d3.select('.context-menu').style('display', 'none');

            });
        });
    }

}

function showPapersAuthored(index,processedArray,activeTab) {
    var paperIds=[];
    var papercompltDetails=[];
    var pids;
    $("#graphArea").css("cursor","wait");

    var intial_length = processedArray.length;
    if (processedArray[index]._source.vType === "author") {
        //  if (processedArray[index]._source.AUTHOR_ID === authorId) {
        _LTracker.push({
            'method':'showPaperedAuthors',
            'tag': 'Authors',
            'value':processedArray[index]
        });

        var urlforpaperId="http://localhost:9200/dblprelation_authorship/_search?q=AUTHOR_ID:"+"\""+processedArray[index]._source.AUTHOR_ID+"\""+"&size=90";
        d3.json(urlforpaperId,function (error,resultIds) {
            if (error) throw error;

            paperIds=resultIds.hits.hits;
            if(paperIds.length>1){
                //http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:("au_53526" OR "au_195445" OR "au_145781" OR "au_93313")
                for(var m=0;m<paperIds.length;m++){

                    if( m<paperIds.length-1){
                        pids=pids+("\""+paperIds[m]._source.PAPER_ID+"\"")+"OR";
                    }
                    else{
                        pids=pids+("\""+paperIds[m]._source.PAPER_ID+"\"");
                    }
                }
            }else if(paperIds.length===1){
                pids=pids+("\""+paperIds[0]._source.PAPER_ID+"\"");
            }

            var urlforPapers="http://localhost:9200/dblpvertexes/_search?q=vType:paper AND PAPER_ID:("+pids.substring(9,pids.length)+")&size=90";
            d3.json(urlforPapers,function (error,resultnames) {
                if (error) throw error;
                papercompltDetails=resultnames.hits.hits;

                for (var j = 0; j < papercompltDetails.length; j++) {
                    if (!paperAlreadyAdded[activeTab].has(papercompltDetails[j]._source.PAPER_ID)) {

                        var idPos = processedArray.push(papercompltDetails[j]);
                        paperAlreadyAdded[activeTab].set(papercompltDetails[j]._source.PAPER_ID, idPos - 1);
                        var newLink = new createLinks(index, processedArray.length - 1,"has_paper");
                        linksArray[activeTab].push(newLink);
                    }else{
                        var newLink = new createLinks(index, paperAlreadyAdded[activeTab].get(papercompltDetails[j]._source.PAPER_ID),"has_paper");
                        linksArray[activeTab].push(newLink);
                    }
                }
                JSON.stringify(linksArray);
                JSON.stringify(processedArray);
                addedSVGs=d3.selectAll("svg");
                idName= "#"+addedSVGs[0][activeTab].getAttribute("id");
                $(idName).empty();

                createGraph(processedArray, linksArray[activeTab],false,false,activeTab);
                $("#graphArea").css("cursor","default");
                d3.select('.context-menu').style('display', 'none');

            });
        });


        // }
    }

}

function showCoAuthorship(index,processedArray,activeTab) {
    $("#graphArea").css("cursor", "wait");
    var cauids;
    var coAuthorIds=[];
    var coAuthorcmpltdetails=[];
    var auids;
    var mainAuthorsOfcoauthorIds=[];
    var mainAuthorsOfcoauthorcmpltDetails=[];
    var skipLoadingCoAuthor=false;
    var urlforcoAuthors;
    var urlformainAuthorsOfcoAuthors;
    var intial_length = processedArray.length;

    if (processedArray[index]._source.vType ===vertexType.AUTHOR) {
        _LTracker.push({
            'method': 'showPaperedAuthors',
            'tag': 'Authors',
            'value': processedArray[index]
        });
    }
    var urlcoAuthors="http://localhost:9200/dblprelation_coauthorship/_search?q=AUTHOR_ID:"+"\""+processedArray[index]._source.AUTHOR_ID+"\"&size=100";
    d3.json(urlcoAuthors,function (error,resultIds) {
        if (error) throw error;

        coAuthorIds = resultIds.hits.hits;
        if(coAuthorIds.length>0){
            for(var m=0;m<coAuthorIds.length;m++){

                if( m<coAuthorIds.length-1){
                    cauids=cauids+("\""+coAuthorIds[m]._source.COAUTHOR_ID+"\"")+"OR";
                }
                else{
                    cauids=cauids+("\""+coAuthorIds[m]._source.COAUTHOR_ID+"\"");
                }
            }
            urlforcoAuthors = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:(" + cauids.substring(9, cauids.length) + ")&size=100";
        }else{
            urlforcoAuthors = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:"+ "\""+"\"";
        }

        d3.json(urlforcoAuthors, function (error, resultnames) {
            if (error) throw error;
            coAuthorcmpltdetails = resultnames.hits.hits;

            for (var j = 0; j < coAuthorcmpltdetails.length; j++) {
                if (!authorAlreadyAdded[activeTab].has(coAuthorcmpltdetails[j]._source.AUTHOR_NAME)) {

                    var idPos = processedArray.push(coAuthorcmpltdetails[j]);
                    authorAlreadyAdded[activeTab].set(coAuthorcmpltdetails[j]._source.AUTHOR_NAME, idPos - 1);
                    var newLink = new createLinks(index, processedArray.length - 1, "coauthor");
                    linksArray[activeTab].push(newLink);
                } else {
                    var newLink = new createLinks(index, authorAlreadyAdded[activeTab].get(coAuthorcmpltdetails[j]._source.AUTHOR_NAME), "");
                    linksArray[activeTab].push(newLink);
                }
            }

            //Querying for authors for whom selected author maybe a coauthor
            var urlmainAuthorsOfcoAuthors = "http://localhost:9200/dblprelation_coauthorship/_search?q=COAUTHOR_ID:" + "\"" + processedArray[index]._source.AUTHOR_ID + "\"size=100";
            d3.json(urlmainAuthorsOfcoAuthors, function (error, resultIds) {
                if (error) throw error;

                mainAuthorsOfcoauthorIds = resultIds.hits.hits;
                if (mainAuthorsOfcoauthorIds.length > 0) {
                    for (var m = 0; m < mainAuthorsOfcoauthorIds.length; m++) {

                        if (m < mainAuthorsOfcoauthorIds.length - 1) {
                            auids = auids + ("\"" + mainAuthorsOfcoauthorIds[m]._source.AUTHOR_ID + "\"") + "OR";
                        }
                        else {
                            auids = auids + ("\"" + mainAuthorsOfcoauthorIds[m]._source.AUTHOR_ID + "\"");
                        }
                    }
                    urlformainAuthorsOfcoAuthors = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:(" + auids.substring(9, auids.length) + ")&size=100";

                }else{

                    urlformainAuthorsOfcoAuthors = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:" + "\""+"\"";
                }

                d3.json(urlformainAuthorsOfcoAuthors, function (error, resultnames) {
                    if (error) throw error;
                    mainAuthorsOfcoauthorcmpltDetails = resultnames.hits.hits;

                    if(mainAuthorsOfcoauthorcmpltDetails.length>0) {
                        for (var j = 0; j < mainAuthorsOfcoauthorcmpltDetails.length; j++) {
                            if (!authorAlreadyAdded[activeTab].has(mainAuthorsOfcoauthorcmpltDetails[j]._source.AUTHOR_NAME)) {
                                var idPos = processedArray.push(mainAuthorsOfcoauthorcmpltDetails[j]);
                                authorAlreadyAdded[activeTab].set(mainAuthorsOfcoauthorcmpltDetails[j]._source.AUTHOR_NAME, idPos - 1);
                                var newLink = new createLinks(index, processedArray.length - 1, "coauthor");
                                linksArray[activeTab].push(newLink);
                            } else {
                                var newLink = new createLinks(index, authorAlreadyAdded[activeTab].get(mainAuthorsOfcoauthorcmpltDetails[j]._source.AUTHOR_NAME), "");
                                linksArray[activeTab].push(newLink);
                            }
                        }
                        JSON.stringify(linksArray);
                        JSON.stringify(processedArray);
                        addedSVGs = d3.selectAll("svg");
                        idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
                        $(idName).empty();
                        update(activeTabIndex);
                        createGraph(processedArray, linksArray[activeTab], false, true, activeTab);
                        $("#graphArea").css("cursor", "default");
                        d3.select('.context-menu').style('display', 'none');
                    }else{
                        JSON.stringify(linksArray);
                        JSON.stringify(processedArray);
                        addedSVGs = d3.selectAll("svg");
                        idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
                        $(idName).empty();
                        update(activeTabIndex);
                        createGraph(processedArray, linksArray[activeTab], false, true, activeTab);
                        $("#graphArea").css("cursor", "default");
                        d3.select('.context-menu').style('display', 'none');

                    }
                });
            });
        });
    });
}

function showAuthorNotCoauthor(index,processedArray,activeTab) {
    var coAuthorIds=[];
    var mainAuthorIds=[];
    var paperIds=[];
    var coAuthorId= new Set();
    var pIds;
    var urlforCitingIds;
    var citingIds=[];
    var cids;
    var urlforCitingAuthors;
    var citingAuthorIds=[];
    var citingAuthorsNotCoauthors=[];
    var aids;
    var urlgetAuthorInfo;

    $("#graphArea").css("cursor", "wait");
    //fill coauthorids into set
    var urlcoAuthors="http://localhost:9200/dblprelation_coauthorship/_search?q=AUTHOR_ID:"+"\""+processedArray[index]._source.AUTHOR_ID+"\"&size=100";
    coAuthorId.add(processedArray[index]._source.AUTHOR_ID);
    d3.json(urlcoAuthors, function (error, resultIds) {
        if (error) throw error;
        coAuthorIds=resultIds.hits.hits;

        for(var m=0;m<coAuthorIds.length;m++){
            coAuthorId.add(coAuthorIds[m]._source.COAUTHOR_ID);
        }
        //fill coauthorids into set
        var urlmainAuthorsOfcoAuthors = "http://localhost:9200/dblprelation_coauthorship/_search?q=COAUTHOR_ID:" + "\"" + processedArray[index]._source.AUTHOR_ID + "\"&size=100";
        d3.json(urlmainAuthorsOfcoAuthors, function (error, resultIds) {
            if (error) throw error;
            mainAuthorIds = resultIds.hits.hits;
            for (var m = 0; m < mainAuthorIds.length; m++) {
                coAuthorId.add(mainAuthorIds[m]._source.AUTHOR_ID);
            }
            //take the authorid and get all his papers
            var urlPaperIds="http://localhost:9200/dblprelation_authorship/_search?q=AUTHOR_ID:\""+processedArray[index]._source.AUTHOR_ID+"\"";
            d3.json(urlPaperIds, function (error, resultIds) {
                if (error) throw error;
                paperIds = resultIds.hits.hits;
                if(paperIds.length>0){
                    for (var m = 0; m < paperIds.length; m++) {
                        if (m < paperIds.length - 1) {
                            pIds = pIds + ("\"" + paperIds[m]._source.PAPER_ID + "\"") + "OR";
                        }
                        else {
                            pIds = pIds + ("\"" + paperIds[m]._source.PAPER_ID + "\"");
                        }
                    }
                    urlforCitingIds = "http://localhost:9200/dblprelation_citation/_search?q=REFERENCE_ID:(" + pIds.substring(9, pIds.length) + ")&size=100";
                }else{
                    urlforCitingIds = "http://localhost:9200/dblprelation_citation/_search?q=REFERENCE_ID:"+ "\""+"\"";
                }

                //take those paper ids and get all papers citing them
                d3.json(urlforCitingIds, function (error, resultIds) {
                    if (error) throw error;
                    citingIds = resultIds.hits.hits;
                    if (citingIds.length > 0) {
                        for (var m = 0; m < citingIds.length; m++) {
                            if (m < citingIds.length - 1) {
                                cids = cids + ("\"" + citingIds[m]._source.PAPER_ID + "\"") + "OR";
                            }
                            else {
                                cids = cids + ("\"" + citingIds[m]._source.PAPER_ID + "\"");
                            }
                        }
                        urlforCitingAuthors = "http://localhost:9200/dblprelation_authorship/_search?q=PAPER_ID:(" + cids.substring(9, cids.length) + ")&size=100";
                    } else {
                        urlforCitingAuthors = "http://localhost:9200/dblprelation_authorship/_search?q=PAPER_ID:" + "\"" + "\"";
                    }

                    //take all this citing papers and get authors of it, check if it already exists in coauthor list and add only if it's not present
                    d3.json(urlforCitingAuthors, function (error, resultIds) {
                        if (error) throw error;
                        citingAuthorIds = resultIds.hits.hits;
                        if (citingAuthorIds.length > 0) {
                            for (var m = 0; m < citingAuthorIds.length; m++) {
                                if (!coAuthorId.has(citingAuthorIds[m]._source.AUTHOR_ID)) {
                                    if (m < citingAuthorIds.length - 1) {
                                        aids = aids + ("\"" + citingAuthorIds[m]._source.AUTHOR_ID + "\"") + "OR";
                                    }
                                    else {
                                        aids = aids + ("\"" + citingAuthorIds[m]._source.AUTHOR_ID + "\"");
                                    }
                                }
                            }
                            if(aids.substring(aids.length-2,aids.length)==="OR"){
                                urlgetAuthorInfo = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:(" + aids.substring(9, aids.length-2) + ")&size=100";
                            }else{
                                urlgetAuthorInfo = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:(" + aids.substring(9, aids.length) + ")&size=100";
                            }
                        } else {
                            urlgetAuthorInfo = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:" + "\"" + "\"";
                        }

                        d3.json(urlgetAuthorInfo, function (error, resultIds) {
                            if (error) throw error;
                            citingAuthorsNotCoauthors = resultIds.hits.hits;
                            if(citingAuthorsNotCoauthors.length>0) {
                                for (var j = 0; j < citingAuthorsNotCoauthors.length; j++) {
                                    if (!authorAlreadyAdded[activeTab].has(citingAuthorsNotCoauthors[j]._source.AUTHOR_NAME)) {
                                        var idPos = processedArray.push(citingAuthorsNotCoauthors[j]);
                                        authorAlreadyAdded[activeTab].set(citingAuthorsNotCoauthors[j]._source.AUTHOR_NAME, idPos - 1);
                                        var newLink = new createLinks(index, processedArray.length - 1, "citing author");
                                        linksArray[activeTab].push(newLink);
                                    } else {
                                        var newLink = new createLinks(index, authorAlreadyAdded[activeTab].get(citingAuthorsNotCoauthors[j]._source.AUTHOR_NAME), "");
                                        linksArray[activeTab].push(newLink);
                                    }
                                }
                                JSON.stringify(linksArray);
                                JSON.stringify(processedArray);
                                addedSVGs = d3.selectAll("svg");
                                idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
                                $(idName).empty();
                                createGraph(processedArray, linksArray[activeTab], false, true, activeTab);
                                $("#graphArea").css("cursor", "default");
                                d3.select('.context-menu').style('display', 'none');
                            }else{
                                JSON.stringify(linksArray);
                                JSON.stringify(processedArray);
                                addedSVGs = d3.selectAll("svg");
                                idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
                                $(idName).empty();
                                createGraph(processedArray, linksArray[activeTab], false, true, activeTab);
                                $("#graphArea").css("cursor", "default");
                                d3.select('.context-menu').style('display', 'none');
                            }
                        });
                    });
                });
            });
        });
    });
}

function showVenue(index,processedArray,activeTab) {
    var venueIds = [];
    var venuecompltDetails = [];
    var venids;


    $("#graphArea").css("cursor", "wait");
    var intial_length = processedArray.length;
    if (processedArray[index]._source.vType === vertexType.PAPER) {
        _LTracker.push({
            'method': 'showVenue',
            'tag': 'venue',
            'value': processedArray[index]
        });

        var urlforvenueId = "http://localhost:9200/dblprelation_venueofpaper/_search?q=PAPER_ID:" + "\"" + processedArray[index]._source.PAPER_ID + "\"";
        d3.json(urlforvenueId, function (error, resultIds) {
            if (error) throw error;
            venueIds=resultIds.hits.hits;
            if(venueIds.length>0){
                for(var m=0;m<venueIds.length;m++){

                    if( m<venueIds.length-1){
                        venids=venids+("\""+venueIds[m]._source.VENUE_ID+"\"")+"OR";
                    }
                    else{
                        venids=venids+("\""+venueIds[m]._source.VENUE_ID+"\"");
                    }
                }
            }
            if(venids!==undefined) {
                var urlforVenueName = "http://localhost:9200/dblpvertexes/_search?q=vType:venue AND VENUE_ID:(" + venids.substring(9, venids.length) + ")";
                d3.json(urlforVenueName, function (error, resultnames) {
                    if (error) throw error;
                    venuecompltDetails = resultnames.hits.hits;

                    for (var j = 0; j < venuecompltDetails.length; j++) {
                        if (!venueAlreadyAdded[activeTab].has(venuecompltDetails[j]._source.VENUE_NAME)) {

                            var idPos = processedArray.push(venuecompltDetails[j]);
                            venueAlreadyAdded[activeTab].set(venuecompltDetails[j]._source.VENUE_NAME, idPos - 1);
                            var newLink = new createLinks(index, processedArray.length - 1, "has_venue");
                            linksArray[activeTab].push(newLink);
                        } else {
                            var newLink = new createLinks(index, venueAlreadyAdded[activeTab].get(venuecompltDetails[j]._source.VENUE_NAME), "has_venue");
                            linksArray[activeTab].push(newLink);
                        }
                    }

                    JSON.stringify(linksArray);
                    JSON.stringify(processedArray);
                    addedSVGs = d3.selectAll("svg");
                    idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
                    $(idName).empty();

                    createGraph(processedArray, linksArray[activeTab], false, false, activeTab);
                    $("#graphArea").css("cursor", "default");
                    d3.select('.context-menu').style('display', 'none');

                });
            }else{
                alert("Venue information of the paper is not available");
                $("#graphArea").css("cursor", "default");
                d3.select('.context-menu').style('display', 'none');

            }
        });
    }
}


function showReferences(index,processedArray,activeTab) {

    var referenceIds=[];
    var referencedPaperdetails=[];
    var refids;
    $("#graphArea").css("cursor", "wait");
    if (processedArray[index]._source.vType ===vertexType.PAPER) {
        _LTracker.push({
            'method': 'showReferences',
            'tag': 'paper',
            'value': processedArray[index]
        });
    }
    var urlforReferenceids="http://localhost:9200/dblprelation_citation/_search?q=PAPER_ID:"+"\""+processedArray[index]._source.PAPER_ID+"\"&size=90";
    d3.json(urlforReferenceids,function (error,resultIds) {
        if (error) throw error;

        referenceIds = resultIds.hits.hits;
        if(referenceIds.length>0){
            for(var m=0;m<referenceIds.length;m++){

                if( m<referenceIds.length-1){
                    refids=refids+("\""+referenceIds[m]._source.REFERENCE_ID+"\"")+"OR";
                }
                else{
                    refids=refids+("\""+referenceIds[m]._source.REFERENCE_ID+"\"");
                }
            }
        }
        if(refids!==undefined) {
            var urlforPaperDetails = "http://localhost:9200/dblpvertexes/_search?q=vType:paper AND PAPER_ID:(" + refids.substring(9, refids.length) + ")&size=90";
            d3.json(urlforPaperDetails, function (error, resultnames) {
                if (error) throw error;
                referencedPaperdetails = resultnames.hits.hits;

                for (var j = 0; j < referencedPaperdetails.length; j++) {
                    if (!paperAlreadyAdded[activeTab].has(referencedPaperdetails[j]._source.PAPER_ID)) {

                        var idPos = processedArray.push(referencedPaperdetails[j]);
                        paperAlreadyAdded[activeTab].set(referencedPaperdetails[j]._source.PAPER_ID, idPos - 1);
                        var newLink = new createLinks(index, processedArray.length - 1, "cites");
                        linksArray[activeTab].push(newLink);
                    } else {
                        var newLink = new createLinks(index, paperAlreadyAdded[activeTab].get(referencedPaperdetails[j]._source.PAPER_ID), "cites");
                        linksArray[activeTab].push(newLink);
                    }
                }

                JSON.stringify(linksArray);
                JSON.stringify(processedArray);
                addedSVGs = d3.selectAll("svg");
                idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
                $(idName).empty();

                createGraph(processedArray, linksArray[activeTab], false, false, activeTab);
                $("#graphArea").css("cursor", "default");
                d3.select('.context-menu').style('display', 'none');
            });
        }else{
            alert("Reference information of the paper is not available");
            $("#graphArea").css("cursor", "default");
            d3.select('.context-menu').style('display', 'none');
        }
    });
}

function showCitations(index,processedArray,activeTab) {

    var citedIds=[];
    var citedPaperdetails=[];
    var cids;
    $("#graphArea").css("cursor", "wait");
    if (processedArray[index]._source.vType ===vertexType.PAPER) {
        _LTracker.push({
            'method': 'showReferences',
            'tag': 'paper',
            'value': processedArray[index]
        });
    }
    var urlforReferenceids="http://localhost:9200/dblprelation_citation/_search?q=REFERENCE_ID:"+"\""+processedArray[index]._source.PAPER_ID+"\"&size=90";
    d3.json(urlforReferenceids,function (error,resultIds) {
        if (error) throw error;

        citedIds = resultIds.hits.hits;
        if(citedIds.length>0){
            for(var m=0;m<citedIds.length;m++){

                if( m<citedIds.length-1){
                    cids=cids+("\""+citedIds[m]._source.PAPER_ID+"\"")+"OR";
                }
                else{
                    cids=cids+("\""+citedIds[m]._source.PAPER_ID+"\"");
                }
            }
        }
        if(cids!==undefined) {
            var urlforPaperDetails = "http://localhost:9200/dblpvertexes/_search?q=vType:paper AND PAPER_ID:(" + cids.substring(9, cids.length) + ")&size=90";
            d3.json(urlforPaperDetails, function (error, resultnames) {
                if (error) throw error;
                citedPaperdetails = resultnames.hits.hits;

                for (var j = 0; j < citedPaperdetails.length; j++) {
                    if (!paperAlreadyAdded[activeTab].has(citedPaperdetails[j]._source.PAPER_ID)) {

                        var idPos = processedArray.push(citedPaperdetails[j]);
                        paperAlreadyAdded[activeTab].set(citedPaperdetails[j]._source.PAPER_ID, idPos - 1);
                        var newLink = new createLinks(processedArray.length - 1,index, "cites");
                        linksArray[activeTab].push(newLink);
                    } else {
                        var newLink = new createLinks(paperAlreadyAdded[activeTab].get(citedPaperdetails[j]._source.PAPER_ID),index, "cites");
                        linksArray[activeTab].push(newLink);
                    }
                }

                JSON.stringify(linksArray);
                JSON.stringify(processedArray);
                addedSVGs = d3.selectAll("svg");
                idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
                $(idName).empty();

                createGraph(processedArray, linksArray[activeTab], false, false, activeTab);
                $("#graphArea").css("cursor", "default");
                d3.select('.context-menu').style('display', 'none');
            });
        }else{
            alert("Citation information of the paper is not available");
            $("#graphArea").css("cursor", "default");
            d3.select('.context-menu').style('display', 'none');
        }
    });
}

function showInstitution(idToEXpand,authorName,indexofCreatedAuthorNode,processedArray,activeTab) {
    var targetId="";
    var orgNames=new Array();
    var url= "http://localhost:9200/janusgraph_edgees/_search?q=eType=authorship AND srcId="+idToEXpand+" AND authorEdge="+authorName;

    $("#graphArea").css("cursor","wait");
    d3.json(url, function (error, json) {
        if (error) throw error;
        // console.log(json)
        _LTracker.push({
            'method':'showInstitution',
            'tag': 'Institution',
            'url':url,
            'execution time':json.took
        });
        if(json.hits.hits.length>0) {
            targetId = json.hits.hits[0]._source.tgtId;
            var getAuthorData = "http://localhost:9200/janusgraph_vertexes/_search?q=vType:author AND jgId:" + targetId;

            d3.json(getAuthorData, function (error, jsonResult) {
                if (error) throw error;
                orgNames = jsonResult.hits.hits[0]._source.orgList;
                _LTracker.push({
                    'method':'showInstitution',
                    'tag': 'Institution',
                    'url':url,
                    'execution time':jsonResult.took
                });

                if ((orgNames != undefined)) {
                    var newNode = new createInstitutionNode(orgNames);
                    var index = processedArray.push(newNode);
                    var newLink = new createLinks(indexofCreatedAuthorNode, processedArray.length - 1);
                    linksArray.push(newLink);
                    instituteAlreadyAdded[activeTab].set(orgNames);
                } else {
                    alert("Membership data not available ")
                }

                JSON.stringify(linksArray);
                JSON.stringify(processedArray);

                addedSVGs=d3.selectAll("svg");
                idName= "#"+addedSVGs[0][activeTab].getAttribute("id");
                $(idName).empty();

                createGraph(processedArray, linksArray[activeTab], false,false,activeTab);
                $("#graphArea").css("cursor", "default");
            });
        }else{
            alert("Membership data not available for selected Author");
            $("#graphArea").css("cursor", "default");
        }
    });

}



function showInstitutionFromAuthor(idToEXpand,selectedIndex,processedArray,activeTabIndex) {
    $("#graphArea").css("cursor","wait");
    if(processedArray[selectedIndex]._source.orgList===undefined){
        alert("Institution information not available")
    }

    if ((processedArray[selectedIndex]._source.vType === vertexType.AUTHOR)&&(processedArray[selectedIndex]._source.orgList!==undefined)) {
        if ((processedArray[selectedIndex]._id === idToEXpand)) {
            _LTracker.push({
                'method':'showInstitutionFromAuthor',
                'tag': 'InstitutionFromAuthor',
                'value':processedArray[selectedIndex]
            });
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

    addedSVGs=d3.selectAll("svg");
    idName= "#"+addedSVGs[0][activeTab].getAttribute("id");
    $(idName).empty();

    createGraph(processedArray, linksArray,false,false,activeTab);
    $("#graphArea").css("cursor","default");
    d3.select('.context-menu').style('display', 'none');
}

function removeNodeAndLinks(selectedIndex,processedArray,linksArray,activeTab) {
    processedArray.splice(selectedIndex,1);

    var initialLength=linksArray.length;
    var i=0;
    while (i<linksArray.length){

        if(linksArray.length>1){
            if((linksArray[i].source.index===selectedIndex)||(linksArray[i].target.index===selectedIndex)){
                linksArray.splice(i,1);
                i=0;
            }else{
                i++;
            }
        }
        else if(linksArray.length===1){
            if((linksArray[i].source.index===selectedIndex)||(linksArray[i].target.index===selectedIndex)){
                linksArray.splice(i,1);
                i=0;
            }else{
                i++;
            }
        }
    }

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);

    d3.selectAll("svg > *").remove();
    createGraph(processedArray, linksArray,false,false,activeTab);
    $("#graphArea").css("cursor","default");
    d3.select('.context-menu').style('display', 'none');
}

function showFOS(idToEXpand,selectedIndex,processedArray,activeTab) {
    $("#graphArea").css("cursor","wait");
    // var intial_length = processedArray.length;
    //for (var i = 0; i < intial_length; i++) {
    if(processedArray[selectedIndex]._source.fosPaper=== undefined){
        alert("Domain information for the selected paper is not available");
    }

    if (processedArray[selectedIndex]._source.vType === vertexType.PAPER) {
        if ((processedArray[selectedIndex]._id === idToEXpand) && (processedArray[selectedIndex]._source.fosPaper!== undefined)) {
            _LTracker.push({
                'method':'showFOS',
                'tag': 'FieldofStudy',
                'value':processedArray[selectedIndex]
            });
            for (var j = 0; j < processedArray[selectedIndex]._source.fosPaper.length; j++) {
                if (!fosAlreadyAdded[activeTab].has(processedArray[selectedIndex]._source.fosPaper[j])) {
                    var newNode = new createFOSNode(processedArray[selectedIndex]._id, processedArray[selectedIndex]._source.fosPaper[j])
                    var index = processedArray.push(newNode);
                    fosAlreadyAdded[activeTab].set(processedArray[selectedIndex]._source.fosPaper[j], index - 1);
                    var newLink = new createLinks(selectedIndex, processedArray.length - 1);
                    linksArray[activeTab].push(newLink);
                }else{
                    var newLink = new createLinks(selectedIndex, fosAlreadyAdded[activeTab].get(processedArray[selectedIndex]._source.fosPaper[j]));
                    linksArray.push(newLink);
                }
            }
        }
    }

    if (processedArray[selectedIndex]._source.vType === vertexType.PAPER) {
        if ((processedArray[selectedIndex]._id !== idToEXpand) && (!paperExpanded.has(processedArray[selectedIndex]._id)) && (processedArray[selectedIndex]._source.fosPaper !== undefined)) {
            for (var j = 0; j < processedArray[selectedIndex]._source.fosPaper.length; j++) {
                if (fosAlreadyAdded[activeTab].has(processedArray[selectedIndex]._source.fosPaper[j].name)) {
                    var newLink = new createLinks(selectedIndex, fosAlreadyAdded[activeTab].get(processedArray[selectedIndex]._source.fosPaper[j].name));
                    linksArray.push(newLink);
                }
            }
        }
    }

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);

    addedSVGs=d3.selectAll("svg");
    idName= "#"+addedSVGs[0][activeTab].getAttribute("id");
    $(idName).empty();

    createGraph(processedArray, linksArray[activeTab],false,false,activeTab);
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
    titleVal_p.innerText=processedArray[idsOfDetailToShow]._source.Title;
    td_titleVal.appendChild(titleVal_p);
    tr_title.appendChild(td_titleVal);
    modalbody.appendChild(tr_title);

    //To bind year of publish
    if(processedArray[idsOfDetailToShow]._source.Year!=="") {
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
        yearVal_p.innerText = processedArray[idsOfDetailToShow]._source.Year;
        td_yearVal.appendChild(yearVal_p);
        tr_year.appendChild(td_yearVal);

        modalbody.appendChild(tr_year);
    }

    //To bind abstract
    if(processedArray[idsOfDetailToShow]._source.Abstract!==""){
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
        abstractVal_p.innerText=processedArray[idsOfDetailToShow]._source.Abstract;
        td_abstractVal.appendChild(abstractVal_p);
        tr_abstract.appendChild(td_abstractVal);
        modalbody.appendChild(tr_abstract);
    }

    //To bind Authors
    if(processedArray[idsOfDetailToShow]._source.Authors!=="") {
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
        authorVal.innerText = processedArray[idsOfDetailToShow]._source.Authors;
        td_authorVal.appendChild(authorVal);
        tr_authors.appendChild(td_authorVal);
        modalbody.appendChild(tr_authors);
    }
    //To bind links
    if(processedArray[idsOfDetailToShow]._source.URL!=="") {
        var tr_url = document.createElement("tr");
        urlHeader = document.createElement("b");
        urlHeader.innerText = "Links : ";
        var td_urlKey = document.createElement("td");
        td_urlKey.setAttribute("id", "tdHeader");
        td_urlKey.appendChild(urlHeader);
        tr_url.appendChild(td_urlKey);
        var td_urlVal = document.createElement("td");
        td_urlVal.setAttribute("id","tdValue");
        var urlArray= new Array();
        urlArray=processedArray[idsOfDetailToShow]._source.URL.split(";");
        for (var i = 0; i < urlArray.length; i++) {
            a = document.createElement("a");
            a.innerText =urlArray[i];
            a.setAttribute("href", urlArray[i]);
            a.setAttribute("target", "_blank");
            td_urlVal.appendChild(a);
            if (i < urlArray.length) {
                linebr = document.createElement("br");
                td_urlVal.appendChild(linebr);
            }
        }
        tr_url.appendChild(td_urlVal);
        modalbody.appendChild(tr_url);

    }

    //To bind FOS
    /* if(processedArray[idsOfDetailToShow]._source.fosPaper!==undefined) {
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
     }*/
    //To bind Venue
    if(processedArray[idsOfDetailToShow]._source.Venue!=="") {
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
        venueVal_p.innerText = processedArray[idsOfDetailToShow]._source.Venue;
        td_venueVal.appendChild(venueVal_p);
        tr_venue.appendChild(td_venueVal);
        modalbody.appendChild(tr_venue);
    }
    //To bind publisher
    if(processedArray[idsOfDetailToShow]._source.Journal!=="") {
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
        publisherVal_p.innerText = processedArray[idsOfDetailToShow]._source.Journal;
        td_publisherVal.appendChild(publisherVal_p);
        tr_publisher.appendChild(td_publisherVal);
        modalbody.appendChild(tr_publisher);
    }
    $("#myModal").modal();
}

function renameTab(element) {
    //console.log(element)
    $('#modalInput').html('');
    document.getElementById("modalInput").value=element.innerText;
    $("#addTagModal").modal();
}

function CloseAddTagWindow() {
    $("#addTagModal").modal('hide');
}


function loadFacets(searchValue,byYear){
    var query;
    var buckets=new Array();
    document.getElementById("ResultsArea").style.visibility="hidden";
    $('#facetResults').html('');
    $("#searchArea").css("cursor","wait");
    if(byYear) {

        query = '{\n' +
            '            "size":0,\n' +
            '            "query":{\n' +
            '                "query_string":{\n' +
            '                    "query":' + JSON.stringify(searchValue) + '\n' +
            '                }\n' +
            '            },\n' +
            '            "aggs":{\n' +
            '                "paperByYear":{\n' +
            '                    "terms":{\n' +
            '                        "field":"year",\n' +
            '                        "size":2000\n' +
            '                    }\n' +
            '                }\n' +
            '            }\n' +
            '        }';
    }else {
        query = '{"size":0,\n' +
            '            "query":{\n' +
            '                "query_string":{\n' +
            '                    "query":' + JSON.stringify(searchValue) + '\n' +
            '                }\n' +
            '            },\n' +
            '            "aggs":{\n' +
            '                "paperByYear":{\n' +
            '                    "terms":{\n' +
            '                        "field":"magNCitation",\n' +
            '                        "size":2000\n' +
            '                    }\n' +
            '                }\n' +
            '            }\n' +
            '        }}';
    }
    $.ajax({
        type: 'POST',
        url: 'http://localhost:9200/_search',
        contentType: 'application/json',
        data: query,
        success: function (response) {
            _LTracker.push({
                'method':'loadFacets',
                'tag': 'Facets',
                'query':query,
                'value':response,
                'timetaken':response.took,
                'totalHits':response.hits.total
            });
            buckets=response.aggregations.paperByYear.buckets;
            console.log(response.aggregations.paperByYear.buckets);

            var facetTab=document.getElementById("facetResults");
            var tr_header=document.createElement("tr");
            var td_year=document.createElement("th");
            td_year.setAttribute("id","setFont");
            if(byYear){
                td_year.innerText="Year";
            }else{
                td_year.innerText="Citation Number";
            }

            var td_docCount=document.createElement("th");
            td_docCount.setAttribute("id","setFont");
            td_docCount.innerText="Document Count";
            tr_header.appendChild(td_year);
            tr_header.appendChild(td_docCount);
            facetTab.appendChild(tr_header);

            for (var i=0;i<buckets.length;i++){
                var tr = document.createElement("tr");
                var tdVal1 = document.createElement("td");
                var tdVal2 = document.createElement("td");

                var yearKey=document.createElement("b");
                var doc_count=document.createElement("b");

                yearKey.innerText=buckets[i].key;
                tdVal1.appendChild(yearKey);
                tdVal1.style.padding="5px";
                doc_count.innerText=buckets[i].doc_count;
                tdVal2.appendChild(doc_count);
                tdVal2.style.padding="5px";
                tr.appendChild(tdVal1);
                tr.appendChild(tdVal2);

                facetTab.appendChild(tr);
                $("#searchArea").css("cursor","default");
            }
        },
        dataType: 'json'
    });

}

function loadResults(){
    document.getElementById("ResultsArea").style.visibility="visible";
}

