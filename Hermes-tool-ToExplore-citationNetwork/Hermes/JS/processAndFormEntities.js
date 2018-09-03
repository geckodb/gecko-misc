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
//varaibles for check entered values
var totalRecords;
var availableRecordCount;
//variables used for showCitations
var citedIds=[];
var citedPaperdetails=[];
var paperWithCitationSeen=[[]];
//variables used for showReferences
var referenceIds=[];
var referencePaperdetails=[];
var paperWithReferenceSeen=[[]];

//variables used for papers authored
var publishedPaperIds=[];
var publishedPaperdetails=[];
var authorWithPublicationSeen=[[]];

//variables used for papers published by journal
var journalpublishedPaperIds=[];
var journalpublishedPaperdetails=[];
var journalWithPublicationSeen=[[]];

//variables used to view coauthorship
var authorWithCoauthorshipSeen=[[]];
var coAtuthorIdsofAuthor=[[]];
var coAuthorDetails=[];

//variables used to view coauthorship
var authorWithCitingNonCoauthorshipSeen=[[]];
var citingNoncoAtuthorIdsofAuthor=[[]];
var nonCoAuthorDetails=[];

authorAlreadyAdded[0]=new Map();
publicationAlreadyAdded[0]=new Map();
venueAlreadyAdded[0]=new Map();
coAuthorsAlreadyAdded[0]=new Map();
fosAlreadyAdded[0]=new Map();
instituteAlreadyAdded[0]=new Map();
paperAlreadyAdded[0]=new Map();

paperWithCitationSeen[0]=new Map();
paperWithReferenceSeen[0]=new Map();

authorWithPublicationSeen[0]=new Map();
authorWithCoauthorshipSeen[0]=new Map();

journalWithPublicationSeen[0]=new Map();

coAtuthorIdsofAuthor[0]=new Map();
authorWithCitingNonCoauthorshipSeen[0]=new Map();
citingNoncoAtuthorIdsofAuthor[0]=new Map();

/**
 * creates links between passed source and target index with a label
 * @param source
 * @param target
 * @param edgeName
 */
function createLinks(source, target,edgeName) {
    this.source = source;
    this.target = target;
    this.label=edgeName;
}

/**
 * shows authors of the selected paper
 * @param idToEXpand
 * @param index
 * @param processedArray
 * @param activeTab
 */
function showAuthors(idToEXpand,index, processedArray,activeTab) {
    var authorIds=[];
    var authorcompltDetails=[];
    var auids;
    var urlforAuthorId;

    d3.select('.context-menu').style('display', 'none');
    $("#graphArea").css("cursor","wait");
    var intial_length = processedArray.length;

    if (processedArray[index]._source.vType === "paper") {

            _LTracker.push({
                'method':'showAuthors',
                'tag': 'Authors-of-Papers',
                'PaperID':processedArray[index]._source.PAPER_ID,
            });

             urlforAuthorId="http://localhost:9200/dblprelation_authorship/_search?q=PAPER_ID:"+"\""+idToEXpand+"\"";
            d3.json(urlforAuthorId,function (error,resultIds) {

                if (error)
                    _LTracker.push({
                        'method':'showAuthors',
                        'tag': 'Error',
                        'PaperID':processedArray[index]._source.PAPER_ID,
                        'Query':urlforAuthorId,
                        'value':error
                    });


                authorIds=resultIds.hits.hits;
                _LTracker.push({
                    'method':'showAuthors',
                    'tag': 'Results-showAuthors-Q1',
                    'Query':urlforAuthorId,
                    'time in ms':resultIds.took,
                    'Author Count':resultIds.hits.total
                });

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
                    if (error)   _LTracker.push({
                        'method':'showAuthors',
                        'tag': 'Error',
                        'Query':urlforAuthorName,
                        'value':error
                    });
                    authorcompltDetails=resultnames.hits.hits;

                    _LTracker.push({
                        'method':'showAuthors',
                        'tag': 'Results-showAuthors-Q2',
                        'Query':urlforAuthorName,
                        'time in ms':resultnames.took,
                        'Author Count':resultnames.hits.total
                    });

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

                    JSON.stringify(linksArray);
                    JSON.stringify(processedArray);
                    addedSVGs=d3.selectAll("svg");
                    idName= "#"+addedSVGs[0][activeTab].getAttribute("id");
                    $(idName).empty();

                    createGraph(processedArray, linksArray[activeTab],false,false,activeTab);
                    $("#graphArea").css("cursor","default");


                });
            });
    }
}

/**
 * shows journal for the selected paper
 * @param index
 * @param processedArray
 * @param activeTab
 */
function showJournals(index, processedArray,activeTab) {
    var journalIds = [];
    var journalcompltDetails = [];
    var jouids;

    d3.select('.context-menu').style('display', 'none');
    $("#graphArea").css("cursor", "wait");

    if (processedArray[index]._source.vType === vertexType.PAPER) {
        _LTracker.push({
            'method': 'showJournals',
            'tag': 'Publisher-of-Paper',
            'PaperID': processedArray[index]._source.PAPER_ID
        });

        var urlforjournalId = "http://localhost:9200/dblprelation_journalofpaper/_search?q=PAPER_ID:" + "\"" + processedArray[index]._source.PAPER_ID + "\"";

        d3.json(urlforjournalId, function (error, resultIds) {

            if (error)
                _LTracker.push({
                    'method':'showJournals',
                    'tag': 'Error',
                    'PaperID': processedArray[index]._source.PAPER_ID,
                    'Query':urlforjournalId,
                    'value':error
                });

            journalIds=resultIds.hits.hits;
            _LTracker.push({
                'method':'showJournals',
                'tag': 'Results-showJournals-Q1',
                'Query':urlforjournalId,
                'time in ms':resultIds.took,
                'Journal Count':resultIds.hits.total
            });

            if(journalIds.length>0){

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

                    if (error)
                        _LTracker.push({
                            'method':'showJournals',
                            'tag': 'Error',
                            'value':error
                        });

                    journalcompltDetails = resultnames.hits.hits;
                    _LTracker.push({
                        'method':'showJournals',
                        'tag': 'Results-showJournals-Q2',
                        'Query':urlforJournalName,
                        'time in ms':resultnames.took,
                        'Journal Count':resultnames.hits.total
                    });

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


                });
            }else{
                alert("Publication of the paper is not available");
                $("#graphArea").css("cursor", "default");

            }
        });
    }
}

/**
 * shows venue node for selected paper
 * @param index
 * @param processedArray
 * @param activeTab
 */
function showVenue(index,processedArray,activeTab) {
    var venueIds = [];
    var venuecompltDetails = [];
    var venids;

    d3.select('.context-menu').style('display', 'none');
    $("#graphArea").css("cursor", "wait");

    var intial_length = processedArray.length;
    if (processedArray[index]._source.vType === vertexType.PAPER) {
        _LTracker.push({
            'method':'showVenue',
            'tag': 'Venue-of-Paper',
            'value':processedArray[index]._source.PAPER_ID
        });

        var urlforvenueId = "http://localhost:9200/dblprelation_venueofpaper/_search?q=PAPER_ID:" + "\"" + processedArray[index]._source.PAPER_ID + "\"";

        d3.json(urlforvenueId, function (error, resultIds) {

            if (error)
                _LTracker.push({
                    'method':'showVenue',
                    'tag': 'Error',
                    'PaperID': processedArray[index]._source.PAPER_ID,
                    'Query':urlforvenueId,
                    'value':error
                });


            venueIds=resultIds.hits.hits;
            _LTracker.push({
                'method':'showVenue',
                'tag': 'Results-showVenue-Q1',
                'Query': urlforvenueId,
                'time in ms':resultIds.took,
                'value':resultIds.hits.total
            });

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

                    if (error)
                        _LTracker.push({
                            'method':'showVenue',
                            'tag': 'Error',
                            'value':error
                        });

                    venuecompltDetails = resultnames.hits.hits;
                    _LTracker.push({
                        'method':'showVenue',
                        'tag': 'Results-showVenue-Q2',
                        'Query':urlforVenueName,
                        'time in ms':resultnames.took,
                        'value':resultnames.hits.total
                    });

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


                });
            }else{
                alert("Venue information of the paper is not available");
                $("#graphArea").css("cursor", "default");
            }
        });
    }
}


/**
 * shows the papers published for a selected journal/publication
 * @param index
 * @param processedArray
 * @param activeTab
 */
function showPapersPublished(index,processedArray,activeTab) {

    d3.select('.context-menu').style('display', 'none');
    $("#graphArea").css("cursor", "wait");

    if (processedArray[index]._source.vType ===vertexType.PUBLICATION) {
        _LTracker.push({
            'method':'showPapersPublished',
            'tag': 'Papers-published',
            'value':processedArray[index]._source.JOURNAL_ID
        });
    }

    var urlforpaperId="http://localhost:9200/dblprelation_journalofpaper/_count?q=JOURNAL_ID:"+"\""+processedArray[index]._source.JOURNAL_ID+"\"";

    $.ajax({
        dataType: "json",
        url: urlforpaperId,
        async: false,
        success: function (dataCount) {
            totalRecords=parseInt(dataCount.count);
        }
    });

    if(totalRecords>0){
        document.getElementById("headerTitle").innerText="Papers Published";
        document.getElementById("totalRec").innerText="Total Records";
        document.getElementById("countofTotalRec").innerText=totalRecords;
        document.getElementById("recInput").value="";
        document.getElementById("OKbtn").classList.add("disabled");
        document.getElementById("OKbtn").style.cursor="not-allowed";
        document.getElementById("warningMsg").style.color="black";
        document.getElementById("recInput").disabled=false;

        if(journalWithPublicationSeen[activeTab].has(processedArray[index]._source.JOURNAL_ID)){
            availableRecordCount= totalRecords - journalWithPublicationSeen[activeTab].get(processedArray[index]._source.JOURNAL_ID);
            document.getElementById("countofAvailableRec").innerText=availableRecordCount;
            document.getElementById("countofDisplayedRec").innerText=journalWithPublicationSeen[activeTab].get(processedArray[index]._source.JOURNAL_ID);

            if(availableRecordCount===0){
                document.getElementById("recInput").disabled=true;
            }
        }else{
            availableRecordCount=totalRecords;
            document.getElementById("countofAvailableRec").innerText=availableRecordCount;
            document.getElementById("countofDisplayedRec").innerText=0;
        }

        document.getElementById("selectedIndexValue").innerText=index;

        $("#showLimitedRecords").modal('show');

    }else{
        alert("Publication information of the journal is not available");
        $("#graphArea").css("cursor", "default");
    }


/*
    var paperIds=[];
    var papercompltDetails=[];
    var pids;

    d3.select('.context-menu').style('display', 'none');
    $("#graphArea").css("cursor","wait");

    var intial_length = processedArray.length;
    if (processedArray[index]._source.vType === "journal") {
        _LTracker.push({
            'method':'showPapersPublished',
            'tag': 'PapersofJournal',
            'value':processedArray[index]._source.JOURNAL_ID
        });


        var urlforpaperId="http://localhost:9200/dblprelation_journalofpaper/_search?q=JOURNAL_ID:"+"\""+processedArray[index]._source.JOURNAL_ID+"\""+"&size=90";
        _LTracker.push({
            'method':'showPapersPublished',
            'tag': 'Queries',
            'url':urlforpaperId
        });

        d3.json(urlforpaperId,function (error,resultIds) {
            if (error)
                _LTracker.push({
                    'method':'showPapersPublished',
                    'tag': 'Error',
                    'value':error
                });

            paperIds=resultIds.hits.hits;
            _LTracker.push({
                'method':'showPapersPublished',
                'tag': 'Results',
                'time in ms':resultIds.took,
                'value':resultIds.hits.hits
            });

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
            _LTracker.push({
                'method':'showPapersPublished',
                'tag': 'Queries',
                'url':urlforPapers
            });
            d3.json(urlforPapers,function (error,resultnames) {
                if (error)
                    _LTracker.push({
                        'method':'showPapersPublished',
                        'tag': 'Error',
                        'value':error
                    });


                papercompltDetails=resultnames.hits.hits;
                _LTracker.push({
                    'method':'showPapersPublished',
                    'tag': 'Results',
                    'time in ms':resultnames.took,
                    'value':resultnames.hits.hits
                });

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

            });
        });
    }*/

}

/**
 *shows modal and available publication count of a Journal, allows user to enter records to be displayed
 * @param activeTab
 * @param index
 * @param processedArray
 */
function okOfPapersPublishedClicked(activeTab,index,processedArray) {
    var pids;
    var retrievalSize;
    var sizeofrecords;
    var refrom;
    journalpublishedPaperdetails=[];

    CloseLimitRecordsWindow();
    $("#graphArea").css("cursor", "wait");
    var enteredVal = parseInt(document.getElementById("recInput").value);
    if(journalWithPublicationSeen[activeTab].has(processedArray[index]._source.JOURNAL_ID)){
        var prevValue=journalWithPublicationSeen[activeTab].get(processedArray[index]._source.JOURNAL_ID);
        journalWithPublicationSeen[activeTab].set(processedArray[index]._source.JOURNAL_ID,parseInt(enteredVal)+parseInt(prevValue));
        refrom=prevValue;
    }else{
        journalWithPublicationSeen[activeTab].set(processedArray[index]._source.JOURNAL_ID,parseInt(enteredVal));
        refrom=0;
    }
    sizeofrecords=enteredVal;

    var loopCnt = Math.ceil(enteredVal / 90);
    for (var x = 0; x < loopCnt; x++) {
        if(sizeofrecords>90){
            retrievalSize=90;
            sizeofrecords=sizeofrecords-90;
        }else{
            retrievalSize=sizeofrecords;
        }
        refrom=(x*90);
        var urlforpaperId="http://localhost:9200/dblprelation_journalofpaper/_search?q=JOURNAL_ID:"+"\""+processedArray[index]._source.JOURNAL_ID+"\"&from="+refrom+"&size="+retrievalSize;

        $.ajax({
            dataType: "json",
            url: urlforpaperId,
            async: false,
            success: function (dataids) {
                _LTracker.push({
                    'method':'okOfPapersPublishedClicked',
                    'tag': 'Results-Papers Published-Q1',
                    'Query':urlforpaperId,
                    'time in ms':dataids.took,
                    'Paper Count':dataids.hits.total

                });
                journalpublishedPaperIds=[];
                journalpublishedPaperIds= journalpublishedPaperIds.concat(dataids.hits.hits);
            }
        });
        pids="";
        for (var m = 0; m <journalpublishedPaperIds.length; m++) {

            if (m < journalpublishedPaperIds.length-1) {
                pids = pids + ("\"" + journalpublishedPaperIds[m]._source.PAPER_ID + "\"") + "OR";
            }
            else {
                pids = pids + ("\"" + journalpublishedPaperIds[m]._source.PAPER_ID + "\"");
            }
        }

        var urlforPaperDetails = "http://localhost:9200/dblpvertexes/_search?q=vType:paper AND PAPER_ID:(" + pids + ")&size="+retrievalSize;

        $.ajax({
            dataType: "json",
            url: urlforPaperDetails,
            async: false,
            success: function (data) {
                _LTracker.push({
                    'method':'okOfPapersPublishedClicked',
                    'tag': 'Results-Papers Published-Q2',
                    'Query':urlforpaperId,
                    'time in ms':data.took,
                    'Paper Count':data.hits.total
                });
                journalpublishedPaperdetails= journalpublishedPaperdetails.concat(data.hits.hits)
            }
        });
    }
    for (var j = 0; j < journalpublishedPaperdetails.length; j++) {
        if (!paperAlreadyAdded[activeTab].has(journalpublishedPaperdetails[j]._source.PAPER_ID)) {

            var idPos = processedArray.push(journalpublishedPaperdetails[j]);
            paperAlreadyAdded[activeTab].set(journalpublishedPaperdetails[j]._source.PAPER_ID, idPos - 1);
            var newLink = new createLinks(index,processedArray.length - 1, "has_paper");
            linksArray[activeTab].push(newLink);
        } else {
            var newLink = new createLinks(index,paperAlreadyAdded[activeTab].get(journalpublishedPaperdetails[j]._source.PAPER_ID), "has_paper");
            linksArray[activeTab].push(newLink);
        }
    }

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);
    addedSVGs = d3.selectAll("svg");
    idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
    $(idName).empty();

    createGraph(processedArray, linksArray[activeTab], false, false, parseInt(activeTab));
    $("#graphArea").css("cursor", "default");
}


/**
 * Returns a set filled with coauthor ids for a given author id
 * @param authorId
 * @returns {Set<any>}
 */
function fillCoAUthorIds(authorId) {
    var totalRecords;
    var coAuthorIds=new Set();
    var urlcoAuthors = "http://localhost:9200/dblprelation_coauthorship/_search?q=AUTHOR_ID:" + "\"" + authorId + "\"&from=0&size=1000";


    $.ajax({
        dataType: "json",
        url: urlcoAuthors,
        async: false,
        success: function (data) {
            totalRecords = parseInt(data.hits.total);
            var ids = [];
            ids = data.hits.hits;
            _LTracker.push({
                'method':'fillCoAUthorIds',
                'tag': 'Results-CoAuthorship-Q1',
                'Query':urlcoAuthors,
                'time in ms':data.took,
                'Paper Count':data.hits.total
            });

            for (var i = 0; i < ids.length; i++) {
                coAuthorIds.add(ids[i]._source.COAUTHOR_ID);
            }

            if (totalRecords > 1000) {
                var loopCnt = Math.ceil(totalRecords / 1000);
                for (var j = 1; j < loopCnt; j++) {
                    $.ajax({
                        dataType: "json",
                        url: "http://localhost:9200/dblprelation_coauthorship/_search?q=AUTHOR_ID:" + "\"" + authorId + "\"&from=" + (j * 1000) + "&size=1000",
                        async: false,
                        success: function (extendeddata) {
                            var extendedIds = [];
                            extendedIds = extendeddata.hits.hits;

                            for (var i = 0; i < extendedIds.length; i++) {
                                coAuthorIds.add(ids[i]._source.COAUTHOR_ID);
                            }
                        }
                    });
                }
            }
        }
    });

    var urlAuthors = "http://localhost:9200/dblprelation_coauthorship/_search?q=COAUTHOR_ID:" + "\"" + authorId + "\"&from=0&size=1000";

    $.ajax({
        dataType: "json",
        url: urlAuthors,
        async: false,
        success: function (data) {
            totalRecords = parseInt(data.hits.total);
            var ids = [];
            ids = data.hits.hits;

            _LTracker.push({
                'method':'fillCoAUthorIds',
                'tag': 'Results-CoAuthorship-Q2',
                'Query':urlAuthors,
                'time in ms':data.took,
                'Paper Count':data.hits.total
            });

            for (var i = 0; i < ids.length; i++) {
                coAuthorIds.add(ids[i]._source.AUTHOR_ID);
            }

            if (totalRecords > 1000) {
                var loopCnt = Math.ceil(totalRecords / 1000);
                for (var j = 1; j < loopCnt; j++) {
                    $.ajax({
                        dataType: "json",
                        url: "http://localhost:9200/dblprelation_coauthorship/_search?q=COAUTHOR_ID:" + "\"" + authorId + "\"&from=" + (j * 1000) + "&size=1000",
                        async: false,
                        success: function (extendeddata) {
                            var extendedIds = [];
                            extendedIds = extendeddata.hits.hits;

                            for (var i = 0; i < extendedIds.length; i++) {
                                coAuthorIds.add(ids[i]._source.AUTHOR_ID);
                            }
                        }
                    });
                }
            }
        }
    });

    return coAuthorIds;
}


/**
 * shows modal and available cited non coauthors count of an author,for this we query citing non-coauthors of a given author and add them into Map with author id as key and set as the value.
 * and also allows user to enter number of records to be displayed
 * @param index
 * @param processedArray
 * @param activeTab
 */
function showAuthorNotCoauthor(index,processedArray,activeTab) {

    var coAuthorIds=[];
    var coAuthorId= new Set();
    var totalRecordCnt;
    var paperIdsofAuthor=[];
    var pids;
    var retrievalSize;
    var sizeofrecords;
    var refrom;
    var start=0;
    var citingIds=[];
    var cids;
    var urlforCitingIds;
    var citedAuthorIds=[];
    var nonCoAuthorId=new Set();



    _LTracker.push({
        'method': 'showAuthorNotCoauthor',
        'tag': 'Citing-Authors',
        'value': processedArray[index]._source.AUTHOR_ID
    });

    if(coAtuthorIdsofAuthor[activeTab].has(processedArray[index]._source.AUTHOR_ID)){
        coAuthorId=coAtuthorIdsofAuthor[activeTab].get(processedArray[index]._source.AUTHOR_ID);
    }
    else {
        coAuthorId=fillCoAUthorIds(processedArray[index]._source.AUTHOR_ID);
        coAtuthorIdsofAuthor[activeTab].set(processedArray[index]._source.AUTHOR_ID,coAuthorId);
    }

    if(citingNoncoAtuthorIdsofAuthor[activeTab].has(processedArray[index]._source.AUTHOR_ID)){
        nonCoAuthorId=citingNoncoAtuthorIdsofAuthor[activeTab].get(processedArray[index]._source.AUTHOR_ID);
    }else {
        //get all papers of the author
        var urlPaperIds = "http://localhost:9200/dblprelation_authorship/_search?q=AUTHOR_ID:\"" + processedArray[index]._source.AUTHOR_ID + "\"&size=1000";
        $.ajax({
            dataType: "json",
            url: urlPaperIds,
            async: false,
            success: function (data) {
                totalRecordCnt = parseInt(data.hits.total);
                var paperIds = [];
                paperIds = data.hits.hits;
                _LTracker.push({
                    'method':'showAuthorNotCoauthor',
                    'tag': 'Results-showAuthorNotCoauthor-Q3',
                    'Query':urlPaperIds,
                    'time in ms':data.took,
                    'Author Count':data.hits.total

                });

                for (var i = 0; i < paperIds.length; i++) {
                    paperIdsofAuthor.push(paperIds[i]._source.PAPER_ID);
                }

                if (totalRecordCnt > 1000) {
                    var loopCnt = Math.ceil(totalRecordCnt / 1000);
                    for (var j = 1; j < loopCnt; j++) {
                        $.ajax({
                            dataType: "json",
                            url: "http://localhost:9200/dblprelation_authorship/_search?q=AUTHOR_ID:\"" + processedArray[index]._source.AUTHOR_ID + "\"&from=" + (j * 1000) + "&size=1000",
                            async: false,
                            success: function (extendeddata) {
                                var extendedIds = [];
                                extendedIds = extendeddata.hits.hits;

                                for (var i = 0; i < extendedIds.length; i++) {
                                    paperIdsofAuthor.push(paperIds[i]._source.PAPER_ID);
                                }
                            }
                        });
                    }
                }
            }
        });


        //get all citing papers for a given set of paper ids
        var loops = Math.ceil(paperIdsofAuthor.length / 90);
        sizeofrecords = paperIdsofAuthor.length;
        for (var k = 0; k < loops; k++) {
            if (sizeofrecords > 90) {
                retrievalSize = 90;
                sizeofrecords = sizeofrecords - 90;
            } else {
                retrievalSize = sizeofrecords;
            }
            pids = "";
            for (var l = start; l < (start + retrievalSize); l++) {
                if (l < (start + retrievalSize) - 1) {
                    pids = pids + ("\"" + paperIdsofAuthor[l] + "\"") + "OR";
                }
                else {
                    pids = pids + ("\"" + paperIdsofAuthor[l] + "\"");
                }
            }

            var urlforCitingIds = "http://localhost:9200/dblprelation_citation/_search?q=REFERENCE_ID:(" + pids + ")&size=" + retrievalSize;

            start += retrievalSize;

            $.ajax({
                dataType: "json",
                url: urlforCitingIds,
                async: false,
                success: function (data) {

                    _LTracker.push({
                        'method':'showAuthorNotCoauthor',
                        'tag': 'Results-showAuthorNotCoauthor-Q4',
                        'Query':urlforCitingIds,
                        'time in ms':data.took,
                        'Author Count':data.hits.total

                    });

                    citingIds = citingIds.concat(data.hits.hits);

                }
            });
        }

        //get all author ids for a given set of paper ids

        var loop = Math.ceil(citingIds.length / 90);
        sizeofrecords = citingIds.length;
        start = 0;
        retrievalSize = 0;
        for (var k = 0; k < loop; k++) {
            if (sizeofrecords > 90) {
                retrievalSize = 90;
                sizeofrecords = sizeofrecords - 90;
            } else {
                retrievalSize = sizeofrecords;
            }
            cids = "";
            for (var l = start; l < (start + retrievalSize); l++) {
                if (l < (start + retrievalSize) - 1) {
                    cids = cids + ("\"" + citingIds[l]._source.PAPER_ID + "\"") + "OR";
                }
                else {
                    cids = cids + ("\"" + citingIds[l]._source.PAPER_ID + "\"");
                }
            }

            var urlforCitedAuthorIds = "http://localhost:9200/dblprelation_authorship/_search?q=PAPER_ID:(" + cids + ")&size=" + retrievalSize;

            start += retrievalSize;

            $.ajax({
                dataType: "json",
                url: urlforCitedAuthorIds,
                async: false,
                success: function (data) {
                    _LTracker.push({
                        'method':'showAuthorNotCoauthor',
                        'tag': 'Results-showAuthorNotCoauthor-Q5',
                        'Query':urlforCitedAuthorIds,
                        'time in ms':data.took,
                        'Author Count':data.hits.total

                    });
                    citedAuthorIds = citedAuthorIds.concat(data.hits.hits);
                }
            });
        }

        //check if obtained citedAuthorIds are in coAuthorId set and store only which are not

        for (var i = 0; i < citedAuthorIds.length; i++) {
            if ((!coAuthorId.has(citedAuthorIds[i]._source.AUTHOR_ID))&&!(citedAuthorIds[i]._source.AUTHOR_ID===processedArray[index]._source.AUTHOR_ID)) {
                nonCoAuthorId.add(citedAuthorIds[i]._source.AUTHOR_ID);
            }
        }
    }

    if(nonCoAuthorId.size>0){
        document.getElementById("headerTitle").innerText="Citing non-coauthors";
        document.getElementById("totalRec").innerText="Total Records";
        document.getElementById("countofTotalRec").innerText=nonCoAuthorId.size;
        document.getElementById("recInput").value="";
        document.getElementById("OKbtn").classList.add("disabled");
        document.getElementById("OKbtn").style.cursor="not-allowed";
        document.getElementById("warningMsg").style.color="black";
        document.getElementById("recInput").disabled=false;

        if(authorWithCitingNonCoauthorshipSeen[activeTab].has(processedArray[index]._source.AUTHOR_ID)){
            availableRecordCount= nonCoAuthorId.size - authorWithCitingNonCoauthorshipSeen[activeTab].get(processedArray[index]._source.AUTHOR_ID);
            document.getElementById("countofAvailableRec").innerText=availableRecordCount;
            document.getElementById("countofDisplayedRec").innerText=authorWithCitingNonCoauthorshipSeen[activeTab].get(processedArray[index]._source.AUTHOR_ID);

            if(availableRecordCount===0){
                document.getElementById("recInput").disabled=true;
            }
        }else{
            citingNoncoAtuthorIdsofAuthor[activeTab].set(processedArray[index]._source.AUTHOR_ID,nonCoAuthorId);
            availableRecordCount=nonCoAuthorId.size;
            document.getElementById("countofAvailableRec").innerText=availableRecordCount;
            document.getElementById("countofDisplayedRec").innerText=0;
        }

        document.getElementById("selectedIndexValue").innerText=index;

        $("#showLimitedRecords").modal('show');

    }else{
        alert("Citing non-coauthor information for a selected author is not available");
        $("#graphArea").css("cursor", "default");
    }

     //fill coauthorids into set

    /*d3.json(urlcoAuthors, function (error, resultIds) {
        if (error)




        d3.json(urlmainAuthorsOfcoAuthors, function (error, resultIds) {

            //take the authorid and get all his papers
            var urlPaperIds="http://localhost:9200/dblprelation_authorship/_search?q=AUTHOR_ID:\""+processedArray[index]._source.AUTHOR_ID+"\"&size=90";
            _LTracker.push({
                'method':'showAuthorNotCoauthor',
                'tag': 'Queries',
                'url':urlPaperIds
            });

            d3.json(urlPaperIds, function (error, resultIds) {
                if (error)
                    _LTracker.push({
                        'method':'showAuthorNotCoauthor',
                        'tag': 'Error',
                        'value':error
                    });

                paperIds = resultIds.hits.hits;
                _LTracker.push({
                    'method':'showAuthorNotCoauthor',
                    'tag': 'Results',
                    'time in ms':resultIds.took,
                    'value':resultIds.hits.hits
                });

                if(paperIds.length>0){
                    for (var m = 0; m < paperIds.length; m++) {
                        if (m < paperIds.length - 1) {
                            pIds = pIds + ("\"" + paperIds[m]._source.PAPER_ID + "\"") + "OR";
                        }
                        else {
                            pIds = pIds + ("\"" + paperIds[m]._source.PAPER_ID + "\"");
                        }
                    }
                    urlforCitingIds = "http://localhost:9200/dblprelation_citation/_search?q=REFERENCE_ID:(" + pIds.substring(9, pIds.length) + ")&size=90";
                }else{
                    urlforCitingIds = "http://localhost:9200/dblprelation_citation/_search?q=REFERENCE_ID:"+ "\""+"\"";
                }
                _LTracker.push({
                    'method':'showAuthorNotCoauthor',
                    'tag': 'Queries',
                    'url':urlforCitingIds
                });

                //take those paper ids and get all papers citing them
                d3.json(urlforCitingIds, function (error, resultIds) {
                    if (error)
                        _LTracker.push({
                            'method':'showAuthorNotCoauthor',
                            'tag': 'Error',
                            'value':error
                        });

                    citingIds = resultIds.hits.hits;
                    _LTracker.push({
                        'method':'showAuthorNotCoauthor',
                        'tag': 'Results',
                        'time in ms':resultIds.took,
                        'value':resultIds.hits.hits
                    });

                    if (citingIds.length > 0) {
                        for (var m = 0; m < citingIds.length; m++) {
                            if (m < citingIds.length - 1) {
                                cids = cids + ("\"" + citingIds[m]._source.PAPER_ID + "\"") + "OR";
                            }
                            else {
                                cids = cids + ("\"" + citingIds[m]._source.PAPER_ID + "\"");
                            }
                        }
                        urlforCitingAuthors = "http://localhost:9200/dblprelation_authorship/_search?q=PAPER_ID:(" + cids.substring(9, cids.length) + ")&size=90";
                    } else {
                        urlforCitingAuthors = "http://localhost:9200/dblprelation_authorship/_search?q=PAPER_ID:" + "\"" + "\"";
                    }
                    _LTracker.push({
                        'method':'showAuthorNotCoauthor',
                        'tag': 'Queries',
                        'url':urlforCitingAuthors
                    });

                    //take all this citing papers and get authors of it, check if it already exists in coauthor list and add only if it's not present
                    d3.json(urlforCitingAuthors, function (error, resultIds) {
                        if (error)
                            _LTracker.push({
                                'method':'showAuthorNotCoauthor',
                                'tag': 'Error',
                                'value':error
                            });

                        citingAuthorIds = resultIds.hits.hits;
                        _LTracker.push({
                            'method':'showAuthorNotCoauthor',
                            'tag': 'Results',
                            'time in ms':resultIds.took,
                            'value':resultIds.hits.hits
                        });

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
                                urlgetAuthorInfo = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:(" + aids.substring(9, aids.length-2) + ")&size=90";
                            }else{
                                urlgetAuthorInfo = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:(" + aids.substring(9, aids.length) + ")&size=90";
                            }
                        } else {
                            urlgetAuthorInfo = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:" + "\"" + "\"";
                        }
                        _LTracker.push({
                            'method':'showAuthorNotCoauthor',
                            'tag': 'Queries',
                            'url':urlgetAuthorInfo
                        });

                        d3.json(urlgetAuthorInfo, function (error, resultIds) {
                            if (error)
                                _LTracker.push({
                                    'method':'showAuthorNotCoauthor',
                                    'tag': 'Error',
                                    'value':error
                                });

                            citingAuthorsNotCoauthors = resultIds.hits.hits;
                            _LTracker.push({
                                'method':'showAuthorNotCoauthor',
                                'tag': 'Results',
                                'time in ms':resultIds.took,
                                'value':resultIds.hits.hits
                            });

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

                            }else{
                                JSON.stringify(linksArray);
                                JSON.stringify(processedArray);
                                addedSVGs = d3.selectAll("svg");
                                idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
                                $(idName).empty();
                                createGraph(processedArray, linksArray[activeTab], false, true, activeTab);
                                $("#graphArea").css("cursor", "default");

                            }
                        });
                    });
                });
            });
        });
    });*/
}

/**
 * Synchronous http requests to get the citing non-coauthor details of a particular author,the authorids in nonCoAuthorDetails set is used  (from, size are calculated based on entered value)
 * @param activeTab
 * @param index
 * @param processedArray
 */
function okOfNonCoAuthorshipClicked(activeTab,index,processedArray) {
    var aids;
    var retrievalSize;
    var sizeofrecords;
    var refrom;
    var start=0;
    var coAuthorshipIds=[];
    nonCoAuthorDetails=[];

    CloseLimitRecordsWindow();
    $("#graphArea").css("cursor", "wait");
    var enteredVal = parseInt(document.getElementById("recInput").value);
    if(authorWithCitingNonCoauthorshipSeen[activeTab].has(processedArray[index]._source.AUTHOR_ID)){
        var prevValue=authorWithCitingNonCoauthorshipSeen[activeTab].get(processedArray[index]._source.AUTHOR_ID);
        authorWithCitingNonCoauthorshipSeen[activeTab].set(processedArray[index]._source.AUTHOR_ID,parseInt(enteredVal)+parseInt(prevValue));
        refrom=prevValue;
    }else{
        authorWithCitingNonCoauthorshipSeen[activeTab].set(processedArray[index]._source.AUTHOR_ID,parseInt(enteredVal));
        refrom=0;
    }

    coAuthorshipIds=Array.from(citingNoncoAtuthorIdsofAuthor[activeTab].get(processedArray[index]._source.AUTHOR_ID));
    aids="";
    if(enteredVal<=100){

        for (var m = refrom; m <(enteredVal+refrom); m++) {

            if (m < enteredVal-1) {
                aids = aids + ("\"" + coAuthorshipIds[m]+ "\"") + "OR";
            }
            else {
                aids = aids + ("\"" + coAuthorshipIds[m]+ "\"");
            }
        }

        var urlforAuthorDetails = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:(" + aids + ")&size="+enteredVal;

        $.ajax({
            dataType: "json",
            url: urlforAuthorDetails,
            async: false,
            success: function (data) {
                nonCoAuthorDetails= nonCoAuthorDetails.concat(data.hits.hits)
            }
        });

    }else{
        var limitedIds=[];
        var count=0;
        for(var x=refrom;x<(enteredVal+refrom);x++){
            limitedIds[count++]=coAuthorshipIds[x];
        }

        var loops=Math.ceil(enteredVal/100);
        sizeofrecords=limitedIds.length;
        for(var k=0;k<loops;k++){
            if(sizeofrecords>100){
                retrievalSize=100;
                sizeofrecords=sizeofrecords-100;
            }else{
                retrievalSize=sizeofrecords;
            }
            aids="";
            for(var l=start;l<(start+retrievalSize);l++){
                if (l< (start+retrievalSize)-1) {
                    aids = aids + ("\"" + limitedIds[l]+ "\"") + "OR";
                }
                else {
                    aids = aids + ("\"" + limitedIds[l]+ "\"");
                }
            }

            var urlforAuthorDetails = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:(" + aids + ")&size="+retrievalSize;

            start+=retrievalSize;

            $.ajax({
                dataType: "json",
                url: urlforAuthorDetails,
                async: false,
                success: function (data) {
                    nonCoAuthorDetails= nonCoAuthorDetails.concat(data.hits.hits)
                }
            });
        }
    }




    for (var j = 0; j < nonCoAuthorDetails.length; j++) {
        if (!authorAlreadyAdded[activeTab].has(nonCoAuthorDetails[j]._source.AUTHOR_NAME)) {

            var idPos = processedArray.push(nonCoAuthorDetails[j]);
            authorAlreadyAdded[activeTab].set(nonCoAuthorDetails[j]._source.AUTHOR_NAME, idPos - 1);
            var newLink = new createLinks(processedArray.length - 1,index, "citing author");
            linksArray[activeTab].push(newLink);
        } else {
            var newLink = new createLinks(authorAlreadyAdded[activeTab].get(nonCoAuthorDetails[j]._source.AUTHOR_NAME),index, "citing author");
            linksArray[activeTab].push(newLink);
        }
    }

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);
    addedSVGs = d3.selectAll("svg");
    idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
    $(idName).empty();

    createGraph(processedArray, linksArray[activeTab], false, true, parseInt(activeTab));
    $("#graphArea").css("cursor", "default");
}


/**
 * shows modal and available coauthorship count of an author,for this we query for coauthors and for authors when given author is coauthor and add in a set to avoid duplicates
 * allows user to enter number of records to be displayed
 * @param index
 * @param processedArray
 * @param activeTab
 */
function showCoAuthorship(index,processedArray,activeTab) {
    var coauthorIds=new Set();

    d3.select('.context-menu').style('display', 'none');
    $("#graphArea").css("cursor", "wait");

    _LTracker.push({
        'method': 'showCoAuthorship',
        'tag': 'Co-authors',
        'authorid':processedArray[index]._source.AUTHOR_ID
    });

    if(coAtuthorIdsofAuthor[activeTab].has(processedArray[index]._source.AUTHOR_ID)){
        coauthorIds=coAtuthorIdsofAuthor[activeTab].get(processedArray[index]._source.AUTHOR_ID);
    }
    else {
            coauthorIds=fillCoAUthorIds(processedArray[index]._source.AUTHOR_ID);
    }
    if(coauthorIds.size>0){
        document.getElementById("headerTitle").innerText="Co-authorship";
        document.getElementById("totalRec").innerText="Total Records";
        document.getElementById("countofTotalRec").innerText=coauthorIds.size;
        document.getElementById("recInput").value="";
        document.getElementById("OKbtn").classList.add("disabled");
        document.getElementById("OKbtn").style.cursor="not-allowed";
        document.getElementById("warningMsg").style.color="black";
        document.getElementById("recInput").disabled=false;

        if(authorWithCoauthorshipSeen[activeTab].has(processedArray[index]._source.AUTHOR_ID)){
            availableRecordCount= coauthorIds.size - authorWithCoauthorshipSeen[activeTab].get(processedArray[index]._source.AUTHOR_ID);
            document.getElementById("countofAvailableRec").innerText=availableRecordCount;
            document.getElementById("countofDisplayedRec").innerText=authorWithCoauthorshipSeen[activeTab].get(processedArray[index]._source.AUTHOR_ID);

            if(availableRecordCount===0){
                document.getElementById("recInput").disabled=true;
            }
        }else{
            coAtuthorIdsofAuthor[activeTab].set(processedArray[index]._source.AUTHOR_ID,coauthorIds);
            availableRecordCount=coauthorIds.size;
            document.getElementById("countofAvailableRec").innerText=availableRecordCount;
            document.getElementById("countofDisplayedRec").innerText=0;
        }

        document.getElementById("selectedIndexValue").innerText=index;

        $("#showLimitedRecords").modal('show');

    }else{
        alert("Co-authorship information of the author is not available");
        $("#graphArea").css("cursor", "default");
    }

}
/**
 * Synchronous http requests to get the coauthor details of a particular author,the authorids in set is used  (from, size are calculated based on entered value)
 * @param activeTab
 * @param index
 * @param processedArray
 */
function okOfCoAuthorshipClicked(activeTab,index,processedArray) {
    var aids;
    var retrievalSize;
    var sizeofrecords;
    var refrom;
    var start=0;
    var coAuthorshipIds=[];
    coAuthorDetails=[];

    CloseLimitRecordsWindow();
    $("#graphArea").css("cursor", "wait");
    var enteredVal = parseInt(document.getElementById("recInput").value);
    if(authorWithCoauthorshipSeen[activeTab].has(processedArray[index]._source.AUTHOR_ID)){
        var prevValue=authorWithCoauthorshipSeen[activeTab].get(processedArray[index]._source.AUTHOR_ID);
        authorWithCoauthorshipSeen[activeTab].set(processedArray[index]._source.AUTHOR_ID,parseInt(enteredVal)+parseInt(prevValue));
        refrom=prevValue;
    }else{
        authorWithCoauthorshipSeen[activeTab].set(processedArray[index]._source.AUTHOR_ID,parseInt(enteredVal));
        refrom=0;
    }

    coAuthorshipIds=Array.from(coAtuthorIdsofAuthor[activeTab].get(processedArray[index]._source.AUTHOR_ID));
        aids="";
        if(enteredVal<=100){

        for (var m = refrom; m <(enteredVal+refrom); m++) {

            if (m < enteredVal-1) {
                aids = aids + ("\"" + coAuthorshipIds[m]+ "\"") + "OR";
            }
            else {
                aids = aids + ("\"" + coAuthorshipIds[m]+ "\"");
            }
        }

        var urlforAuthorDetails = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:(" + aids + ")&size="+enteredVal;

        $.ajax({
            dataType: "json",
            url: urlforAuthorDetails,
            async: false,
            success: function (data) {
                coAuthorDetails= coAuthorDetails.concat(data.hits.hits)
            }
        });

    }else{
            var limitedIds=[];
            var count=0;
            for(var x=refrom;x<(enteredVal+refrom);x++){
                limitedIds[count++]=coAuthorshipIds[x];
            }

            var loops=Math.ceil(enteredVal/100);
            sizeofrecords=limitedIds.length;
            for(var k=0;k<loops;k++){
                if(sizeofrecords>100){
                    retrievalSize=100;
                    sizeofrecords=sizeofrecords-100;
                }else{
                    retrievalSize=sizeofrecords;
                }
                aids="";
                for(var l=start;l<(start+retrievalSize);l++){
                    if (l< (start+retrievalSize)-1) {
                        aids = aids + ("\"" + limitedIds[l]+ "\"") + "OR";
                    }
                    else {
                        aids = aids + ("\"" + limitedIds[l]+ "\"");
                    }
                }

                var urlforAuthorDetails = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_ID:(" + aids + ")&size="+retrievalSize;

                start+=retrievalSize;

                $.ajax({
                    dataType: "json",
                    url: urlforAuthorDetails,
                    async: false,
                    success: function (data) {
                        coAuthorDetails= coAuthorDetails.concat(data.hits.hits)
                    }
                });
            }
        }




    for (var j = 0; j < coAuthorDetails.length; j++) {
        if (!authorAlreadyAdded[activeTab].has(coAuthorDetails[j]._source.AUTHOR_NAME)) {

            var idPos = processedArray.push(coAuthorDetails[j]);
            authorAlreadyAdded[activeTab].set(coAuthorDetails[j]._source.AUTHOR_NAME, idPos - 1);
            var newLink = new createLinks(index,processedArray.length - 1, "coauthor");
            linksArray[activeTab].push(newLink);
        } else {
            var newLink = new createLinks(index,authorAlreadyAdded[activeTab].get(coAuthorDetails[j]._source.AUTHOR_NAME), "coauthor");
            linksArray[activeTab].push(newLink);
        }
    }

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);
    addedSVGs = d3.selectAll("svg");
    idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
    $(idName).empty();

    createGraph(processedArray, linksArray[activeTab], false, false, parseInt(activeTab));
    $("#graphArea").css("cursor", "default");
}


/**
 * shows modal and available publication count of an author, allows user to enter records to be displayed
 * @param index
 * @param processedArray
 * @param activeTab
 */
function showPapersAuthored(index,processedArray,activeTab) {

    d3.select('.context-menu').style('display', 'none');
    $("#graphArea").css("cursor", "wait");

    if (processedArray[index]._source.vType ===vertexType.AUTHOR) {
        _LTracker.push({
            'method':'showPapersAuthored',
            'tag': 'Papers-of-Author',
            'value':processedArray[index]._source.AUTHOR_ID
        });
    }

    var urlforpaperId="http://localhost:9200/dblprelation_authorship/_count?q=AUTHOR_ID:"+"\""+processedArray[index]._source.AUTHOR_ID+"\"";

    $.ajax({
        dataType: "json",
        url: urlforpaperId,
        async: false,
        success: function (dataCount) {
            totalRecords=parseInt(dataCount.count);
        }
    });

    if(totalRecords>0){
        document.getElementById("headerTitle").innerText="Publications";
        document.getElementById("totalRec").innerText="Total Records";
        document.getElementById("countofTotalRec").innerText=totalRecords;
        document.getElementById("recInput").value="";
        document.getElementById("OKbtn").classList.add("disabled");
        document.getElementById("OKbtn").style.cursor="not-allowed";
        document.getElementById("warningMsg").style.color="black";
        document.getElementById("recInput").disabled=false;

        if(authorWithPublicationSeen[activeTab].has(processedArray[index]._source.AUTHOR_ID)){
            availableRecordCount= totalRecords - authorWithPublicationSeen[activeTab].get(processedArray[index]._source.AUTHOR_ID);
            document.getElementById("countofAvailableRec").innerText=availableRecordCount;
            document.getElementById("countofDisplayedRec").innerText=authorWithPublicationSeen[activeTab].get(processedArray[index]._source.AUTHOR_ID);

            if(availableRecordCount===0){
                document.getElementById("recInput").disabled=true;
            }
        }else{
            availableRecordCount=totalRecords;
            document.getElementById("countofAvailableRec").innerText=availableRecordCount;
            document.getElementById("countofDisplayedRec").innerText=0;
        }

        document.getElementById("selectedIndexValue").innerText=index;

        $("#showLimitedRecords").modal('show');

    }else{
        alert("Publication information of the author is not available");
        $("#graphArea").css("cursor", "default");
    }
}

/**
 * Synchronous http requests to get the papers of a particular author,(from, size are calculated based on entered value)
 * @param activeTab
 * @param index
 * @param processedArray
 */
function okOfPapersAuthoredClicked(activeTab,index,processedArray) {
    var pids;
    var retrievalSize;
    var sizeofrecords;
    var refrom;
    publishedPaperdetails=[];

    CloseLimitRecordsWindow();
    $("#graphArea").css("cursor", "wait");
    var enteredVal = parseInt(document.getElementById("recInput").value);
    if(authorWithPublicationSeen[activeTab].has(processedArray[index]._source.AUTHOR_ID)){
        var prevValue=authorWithPublicationSeen[activeTab].get(processedArray[index]._source.AUTHOR_ID);
        authorWithPublicationSeen[activeTab].set(processedArray[index]._source.AUTHOR_ID,parseInt(enteredVal)+parseInt(prevValue));
        refrom=prevValue;
    }else{
        authorWithPublicationSeen[activeTab].set(processedArray[index]._source.AUTHOR_ID,parseInt(enteredVal));
        refrom=0;
    }
    sizeofrecords=enteredVal;

    var loopCnt = Math.ceil(enteredVal / 90);
    for (var x = 0; x < loopCnt; x++) {
        if(sizeofrecords>90){
            retrievalSize=90;
            sizeofrecords=sizeofrecords-90;
        }else{
            retrievalSize=sizeofrecords;
        }
        refrom=(x*90);
        var urlforpaperId="http://localhost:9200/dblprelation_authorship/_search?q=AUTHOR_ID:"+"\""+processedArray[index]._source.AUTHOR_ID+"\"&from="+refrom+"&size="+retrievalSize;

        $.ajax({
            dataType: "json",
            url: urlforpaperId,
            async: false,
            success: function (dataids) {
                _LTracker.push({
                    'method':'okOfPapersAuthoredClicked',
                    'tag': 'Results-PapersAuthored',
                    'Query':urlforpaperId,
                    'time in ms':dataids.took,
                    'Paper Count':dataids.hits.total
                });

                publishedPaperIds=[];
                publishedPaperIds= publishedPaperIds.concat(dataids.hits.hits);
            }
        });
        pids="";
        for (var m = 0; m <publishedPaperIds.length; m++) {

            if (m < publishedPaperIds.length-1) {
                pids = pids + ("\"" + publishedPaperIds[m]._source.PAPER_ID + "\"") + "OR";
            }
            else {
                pids = pids + ("\"" + publishedPaperIds[m]._source.PAPER_ID + "\"");
            }
        }

        var urlforPaperDetails = "http://localhost:9200/dblpvertexes/_search?q=vType:paper AND PAPER_ID:(" + pids + ")&size="+retrievalSize;

        $.ajax({
            dataType: "json",
            url: urlforPaperDetails,
            async: false,
            success: function (data) {
                publishedPaperdetails= publishedPaperdetails.concat(data.hits.hits)
            }
        });
    }
    for (var j = 0; j < publishedPaperdetails.length; j++) {
        if (!paperAlreadyAdded[activeTab].has(publishedPaperdetails[j]._source.PAPER_ID)) {

            var idPos = processedArray.push(publishedPaperdetails[j]);
            paperAlreadyAdded[activeTab].set(publishedPaperdetails[j]._source.PAPER_ID, idPos - 1);
            var newLink = new createLinks(index,processedArray.length - 1, "has_paper");
            linksArray[activeTab].push(newLink);
        } else {
            var newLink = new createLinks(index,paperAlreadyAdded[activeTab].get(publishedPaperdetails[j]._source.PAPER_ID), "has_paper");
            linksArray[activeTab].push(newLink);
        }
    }

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);
    addedSVGs = d3.selectAll("svg");
    idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
    $(idName).empty();

    createGraph(processedArray, linksArray[activeTab], false, false, parseInt(activeTab));
    $("#graphArea").css("cursor", "default");
}



/**
 * shows modal and available reference count, allows user to enter records to be displayed
 * @param index
 * @param processedArray
 * @param activeTab
 */
function showReferences(index,processedArray,activeTab) {

    d3.select('.context-menu').style('display', 'none');
    $("#graphArea").css("cursor", "wait");

    if (processedArray[index]._source.vType ===vertexType.PAPER) {
        _LTracker.push({
            'method': 'showReferences',
            'tag': 'Reference-of-Paper',
            'value': processedArray[index]._source.PAPER_ID
        });
    }

    var urlforReferenceids="http://localhost:9200/dblprelation_citation/_count?q=PAPER_ID:"+"\""+processedArray[index]._source.PAPER_ID+"\"";

    $.ajax({
        dataType: "json",
        url: urlforReferenceids,
        async: false,
        success: function (dataCount) {
            totalRecords=parseInt(dataCount.count);
        }
    });

    if(totalRecords>0){
        document.getElementById("headerTitle").innerText="References"
        document.getElementById("totalRec").innerText="Total Records";
        document.getElementById("countofTotalRec").innerText=totalRecords;
        document.getElementById("recInput").value="";
        document.getElementById("OKbtn").classList.add("disabled");
        document.getElementById("OKbtn").style.cursor="not-allowed";
        document.getElementById("warningMsg").style.color="black";
        document.getElementById("recInput").disabled=false;

        if(paperWithReferenceSeen[activeTab].has(processedArray[index]._source.PAPER_ID)){
            availableRecordCount= totalRecords - paperWithReferenceSeen[activeTab].get(processedArray[index]._source.PAPER_ID);
            document.getElementById("countofAvailableRec").innerText=availableRecordCount;
            document.getElementById("countofDisplayedRec").innerText=paperWithReferenceSeen[activeTab].get(processedArray[index]._source.PAPER_ID);

            if(availableRecordCount===0){
                document.getElementById("recInput").disabled=true;
                /*document.getElementById("OKbtn").classList.add("disabled");
                document.getElementById("OKbtn").style.cursor="not-allowed";*/
            }
        }else{
            availableRecordCount=totalRecords;
            document.getElementById("countofAvailableRec").innerText=availableRecordCount;
            document.getElementById("countofDisplayedRec").innerText=0;
        }

        document.getElementById("selectedIndexValue").innerText=index;

        $("#showLimitedRecords").modal('show');

    }else{
        alert("Reference information of the paper is not available");
        $("#graphArea").css("cursor", "default");
    }

}

/**
 * Synchronous http requests to get the citing papers of a particular paper,(from, size are calculated based on entered value)
 * @param activeTab
 * @param index
 * @param processedArray
 */
function okOfReferencesClicked(activeTab,index,processedArray) {
    var rids;
    var retrievalSize;
    var sizeofrecords;
    var refrom;
    referencePaperdetails=[];

    CloseLimitRecordsWindow();
    $("#graphArea").css("cursor", "wait");
    var enteredVal = parseInt(document.getElementById("recInput").value);
    if(paperWithReferenceSeen[activeTab].has(processedArray[index]._source.PAPER_ID)){
        var prevValue=paperWithReferenceSeen[activeTab].get(processedArray[index]._source.PAPER_ID);
        paperWithReferenceSeen[activeTab].set(processedArray[index]._source.PAPER_ID,parseInt(enteredVal)+parseInt(prevValue));
        refrom=prevValue;
    }else{
        paperWithReferenceSeen[activeTab].set(processedArray[index]._source.PAPER_ID,parseInt(enteredVal));
        refrom=0;
    }
    sizeofrecords=enteredVal;

    var loopCnt = Math.ceil(enteredVal / 90);
    for (var x = 0; x < loopCnt; x++) {
        if(sizeofrecords>90){
            retrievalSize=90;
            sizeofrecords=sizeofrecords-90;
        }else{
            retrievalSize=sizeofrecords;
        }
        refrom=(x*90);
        var urlforReferenceids="http://localhost:9200/dblprelation_citation/_search?q=PAPER_ID:"+"\""+processedArray[index]._source.PAPER_ID+"\"&from="+refrom+"&size="+retrievalSize;
        $.ajax({
            dataType: "json",
            url: urlforReferenceids,
            async: false,
            success: function (dataids) {
                _LTracker.push({
                    'method':'okOfReferencesClicked',
                    'tag': 'Results-PapersReferred',
                    'Query':urlforReferenceids,
                    'time in ms':dataids.took,
                    'Paper Count':dataids.hits.total
                });

                referenceIds=[];
                referenceIds= referenceIds.concat(dataids.hits.hits);
            }
        });
        rids="";
        for (var m = 0; m <referenceIds.length; m++) {

            if (m < referenceIds.length-1) {
                rids = rids + ("\"" + referenceIds[m]._source.REFERENCE_ID + "\"") + "OR";
            }
            else {
                rids = rids + ("\"" + referenceIds[m]._source.REFERENCE_ID + "\"");
            }
        }

        var urlforPaperDetails = "http://localhost:9200/dblpvertexes/_search?q=vType:paper AND PAPER_ID:(" + rids + ")&size="+retrievalSize;
        _LTracker.push({
            'method': 'showCitations',
            'tag': 'Queries',
            'url': urlforPaperDetails
        });

        $.ajax({
            dataType: "json",
            url: urlforPaperDetails,
            async: false,
            success: function (data) {
                referencePaperdetails= referencePaperdetails.concat(data.hits.hits)
            }
        });
    }
    for (var j = 0; j < referencePaperdetails.length; j++) {
        if (!paperAlreadyAdded[activeTab].has(referencePaperdetails[j]._source.PAPER_ID)) {

            var idPos = processedArray.push(referencePaperdetails[j]);
            paperAlreadyAdded[activeTab].set(referencePaperdetails[j]._source.PAPER_ID, idPos - 1);
            var newLink = new createLinks(index,processedArray.length - 1, "cites");
            linksArray[activeTab].push(newLink);
        } else {
            var newLink = new createLinks(index,paperAlreadyAdded[activeTab].get(referencePaperdetails[j]._source.PAPER_ID), "cites");
            linksArray[activeTab].push(newLink);
        }
    }

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);
    addedSVGs = d3.selectAll("svg");
    idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
    $(idName).empty();

    createGraph(processedArray, linksArray[activeTab], false, false, parseInt(activeTab));
    $("#graphArea").css("cursor", "default");
}


/**
 * shows modal and available citation count, allows user to enter records to be displayed
 * @param index
 * @param processedArray
 * @param activeTab
 */
function showCitations(index,processedArray,activeTab) {

    var selIndex=index;

    d3.select('.context-menu').style('display', 'none');
    $("#graphArea").css("cursor", "wait");

    if (processedArray[index]._source.vType ===vertexType.PAPER) {
        _LTracker.push({
            'method': 'showCitations',
            'tag': 'Citations-of-papers',
            'value': processedArray[index]._source.PAPER_ID
        });
    }

    var urlforReferenceids="http://localhost:9200/dblprelation_citation/_count?q=REFERENCE_ID:"+"\""+processedArray[index]._source.PAPER_ID+"\"";

    $.ajax({
        dataType: "json",
        url: urlforReferenceids,
        async: false,
        success: function (dataCount) {
            totalRecords=parseInt(dataCount.count);
        }
    });

        if(totalRecords>0){
            document.getElementById("headerTitle").innerText="Citations"
            //document.getElementById("totalRec").innerText="Total Records";
            document.getElementById("countofTotalRec").innerText=totalRecords;
            document.getElementById("recInput").value="";
            document.getElementById("OKbtn").classList.add("disabled");
            document.getElementById("OKbtn").style.cursor="not-allowed";
            document.getElementById("warningMsg").style.color="black";
            document.getElementById("recInput").disabled=false;

            if(paperWithCitationSeen[activeTab].has(processedArray[index]._source.PAPER_ID)){
                availableRecordCount= totalRecords - paperWithCitationSeen[activeTab].get(processedArray[index]._source.PAPER_ID);
                document.getElementById("countofAvailableRec").innerText=availableRecordCount;
                document.getElementById("countofDisplayedRec").innerText=paperWithCitationSeen[activeTab].get(processedArray[index]._source.PAPER_ID);

                if(availableRecordCount===0){
                    document.getElementById("recInput").disabled=true;
                    /*document.getElementById("OKbtn").classList.add("disabled");
                    document.getElementById("OKbtn").style.cursor="not-allowed";*/
                }
            }else{
                availableRecordCount=totalRecords;
                document.getElementById("countofAvailableRec").innerText=availableRecordCount;
                document.getElementById("countofDisplayedRec").innerText=0;
            }

            document.getElementById("selectedIndexValue").innerText=index;

           $("#showLimitedRecords").modal('show');

            //document.getElementById("OKbtn").addEventListener("click",okClicked,{once: true});*/
            //$("#OKbtn").click(okClicked,{once:true});

        }else{
            alert("Citation information of the paper is not available");
            $("#graphArea").css("cursor", "default");
        }
}

/**
 * Synchronous http requests to get the citing papers of a particular paper,(from, size are calculated based on entered value)
 * @param activeTab
 * @param index
 * @param processedArray
 */
function okOfCitationsClicked(activeTab,index,processedArray) {
    var cids;
    var retrievalSize;
    var sizeofrecords;
    var refrom;
    citedPaperdetails=[];
    CloseLimitRecordsWindow();
    $("#graphArea").css("cursor", "wait");
    document.getElementById("OKbtn").style.cursor="wait";
    var enteredVal = parseInt(document.getElementById("recInput").value);
    if(paperWithCitationSeen[activeTab].has(processedArray[index]._source.PAPER_ID)){
        var prevValue=paperWithCitationSeen[activeTab].get(processedArray[index]._source.PAPER_ID);
        paperWithCitationSeen[activeTab].set(processedArray[index]._source.PAPER_ID,parseInt(enteredVal)+parseInt(prevValue));
        refrom=prevValue;
    }else{
        paperWithCitationSeen[activeTab].set(processedArray[index]._source.PAPER_ID,parseInt(enteredVal));
        refrom=0;
    }
            sizeofrecords=enteredVal;

            var loopCnt = Math.ceil(enteredVal / 90);
            for (var x = 0; x < loopCnt; x++) {
                if(sizeofrecords>90){
                    retrievalSize=90;
                    sizeofrecords=sizeofrecords-90;
                }else{
                    retrievalSize=sizeofrecords;
                }
                 refrom=(x*90);
                var urlforReferenceids="http://localhost:9200/dblprelation_citation/_search?q=REFERENCE_ID:"+"\""+processedArray[index]._source.PAPER_ID+"\"&from="+refrom+"&size="+retrievalSize;
                $.ajax({
                    dataType: "json",
                    url: urlforReferenceids,
                    async: false,
                    success: function (dataids) {
                        _LTracker.push({
                            'method':'okOfCitationsClicked',
                            'tag': 'Results-PapersCited',
                            'Query':urlforReferenceids,
                            'time in ms':dataids.took,
                            'Paper Count':dataids.hits.total
                        });
                        citedIds=[];
                        citedIds= citedIds.concat(dataids.hits.hits);
                    }
                });
                cids="";
                for (var m = 0; m <citedIds.length; m++) {

                    if (m < citedIds.length-1) {
                        cids = cids + ("\"" + citedIds[m]._source.PAPER_ID + "\"") + "OR";
                    }
                    else {
                        cids = cids + ("\"" + citedIds[m]._source.PAPER_ID + "\"");
                    }
                }

                var urlforPaperDetails = "http://localhost:9200/dblpvertexes/_search?q=vType:paper AND PAPER_ID:(" + cids + ")&size="+retrievalSize;

                $.ajax({
                    dataType: "json",
                    url: urlforPaperDetails,
                    async: false,
                    success: function (data) {
                        citedPaperdetails= citedPaperdetails.concat(data.hits.hits)
                    }
                });
            }
            for (var j = 0; j < citedPaperdetails.length; j++) {
                if (!paperAlreadyAdded[activeTab].has(citedPaperdetails[j]._source.PAPER_ID)) {

                    var idPos = processedArray.push(citedPaperdetails[j]);
                    paperAlreadyAdded[activeTab].set(citedPaperdetails[j]._source.PAPER_ID, idPos - 1);
                    var newLink = new createLinks(processedArray.length - 1, index, "cites");
                    linksArray[activeTab].push(newLink);
                } else {
                    var newLink = new createLinks(paperAlreadyAdded[activeTab].get(citedPaperdetails[j]._source.PAPER_ID), index, "cites");
                    linksArray[activeTab].push(newLink);
                }
            }

            JSON.stringify(linksArray);
            JSON.stringify(processedArray);
            addedSVGs = d3.selectAll("svg");
            idName = "#" + addedSVGs[0][activeTab].getAttribute("id");
            $(idName).empty();
            createGraph(processedArray, linksArray[activeTab], false, false, parseInt(activeTab));
            $("#graphArea").css("cursor", "default");
  }


/**
 * Checks if user entered value to view citation record is within available records
 */
function checkValueEntered() {
    var value=document.getElementById("recInput").value;
    if((value>0) && (value<=availableRecordCount)){
        document.getElementById("OKbtn").classList.remove("disabled");
        document.getElementById("OKbtn").style.cursor="pointer";
        //document.getElementById("warningMsg").style.color="black";
    }else{
        document.getElementById("OKbtn").classList.add("disabled");
        document.getElementById("OKbtn").style.cursor="not-allowed";
        document.getElementById("warningMsg").style.color="Red";
    }
}

/**
 * removes the selected node with its connected links based on selection
 * @param selectedIndex
 * @param processedArray
 * @param linksArray
 * @param activeTab
 */
function removeNodeAndLinks(selectedIndex,processedArray,linksArray,activeTab) {
    //code changes that supports removal of links as well but still needs modifications.
    /*var isSource=false;
    var slicePos=[];
    var initialLength=linksArray.length;
    var i=0;
    var newlyProcessedArray=[];
    processedArray.splice(selectedIndex,1);
    while (i<linksArray.length){

        if(linksArray.length>=1){
            if((linksArray[i].source.index===selectedIndex)){
                    for(var k=i+1;k<linksArray.length;k++){
                        if((linksArray[i].target.index===linksArray[k].source.index)||(linksArray[i].source.index===linksArray[k].target.index)){
                            isSource=true;
                            break;
                        }
                    }
                    if(isSource){
                        linksArray.splice(i,1);
                        isSource=false;
                    }else{
                        if(processedArray[i]._source.vType===vertexType.PAPER){
                            paperAlreadyAdded[activeTab].delete(processedArray[i]._source.PAPER_ID);
                        }else if(processedArray[i]._source.vType===vertexType.AUTHOR){
                            authorAlreadyAdded[activeTab].delete(processedArray[i]._source.AUTHOR_NAME);
                        }
                       //processedArray.splice(i,1);
                        slicePos.push(linksArray[i].target.index);
                        linksArray.splice(i,1);
                    }
                i=0;
            }
            else if((linksArray[i].target.index===selectedIndex)){
                for(var k=i+1;k<linksArray.length;k++){
                    if((linksArray[i].source.index===linksArray[k].source.index)||(linksArray[i].source.index===linksArray[k].target.index)){
                        isSource=true;
                        break;
                    }
                }
                if(isSource){
                    linksArray.splice(i,1);
                    isSource=false;
                }else{
                    if(processedArray[i]._source.vType===vertexType.PAPER){
                        paperAlreadyAdded[activeTab].delete(processedArray[i]._source.PAPER_ID);
                    }else if(processedArray[i]._source.vType===vertexType.AUTHOR){
                        authorAlreadyAdded[activeTab].delete(processedArray[i]._source.AUTHOR_NAME);
                    }
                  // processedArray.splice(i,1);
                    slicePos.push(linksArray[i].source.index);
                    linksArray.splice(i,1);
                }
                i=0;
            }else{
                i++;
            }
        }

    JSON.stringify(linksArray);
    JSON.stringify(processedArray);

    //selects all element of svg
    d3.selectAll("svg > *").remove();
    createGraph(processedArray, linksArray,false,false,activeTab);
    $("#graphArea").css("cursor","default");
    d3.select('.context-menu').style('display', 'none');*/
    //end

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

/**
 * Html elements are dynamically created and data of selected paper is binded
 * @param idsOfDetailToShow
 * @param processedArray
 */
function showCompleteDetails(idsOfDetailToShow,processedArray){

    $('#modalcontents').html('');
    var modalbody=document.getElementById("modalcontents");

    _LTracker.push({
        'method':'showCompleteDetails',
        'tag': 'showCompleteDetails-of-Paper',
        'PaperID':processedArray[idsOfDetailToShow]._source.PAPER_ID,
    });

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
        authorVal.innerText = processedArray[idsOfDetailToShow]._source.Authors.replace(/;/g,", ");
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

/**
 * shows popup to rename selected tab
 * @param element
 */
function renameTab(element) {
    //console.log(element)
    $('#modalInput').html('');
    document.getElementById("modalInput").value=element.innerText;
    $("#addTagModal").modal();
}

/**
 * hides the add tag modal
 * @constructor
 */
function CloseAddTagWindow() {
    $("#addTagModal").modal('hide');
}

/**
 * hides limit record displayed modal
 * @constructor
 */
function CloseLimitRecordsWindow() {
    $("#showLimitedRecords").modal('hide');
}

/**
 *  hides limit record displayed modal and sets cursor to default
 * @constructor
 */
function CancelLimitRecordsWindow() {
    $("#showLimitedRecords").modal('hide');
    $("#graphArea").css("cursor", "default");
}

/**
 * aggregated query for year
 * @param searchValue
 * @param byYear
 */
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
            '                        "field":"Year",\n' +
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
            '                        "field":"CitationCount",\n' +
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
        dataType: 'json',
        async:false,
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
           // console.log(response.aggregations.paperByYear.buckets);


            if(buckets.length>0) {

                var facetTab=document.getElementById("facetResults");
                var tr_header=document.createElement("tr");
                var td_year=document.createElement("th");
                td_year.setAttribute("id","setFont");
                if(byYear){
                    td_year.innerText="Year";
                }else{
                    td_year.innerText="Citation Number";
                }
                var td_docCount = document.createElement("th");
                td_docCount.setAttribute("id", "setFont");
                td_docCount.innerText = "Document Count";
                tr_header.appendChild(td_year);
                tr_header.appendChild(td_docCount);
                facetTab.appendChild(tr_header);

                for (var i = 0; i < buckets.length; i++) {
                    var tr = document.createElement("tr");
                    var tdVal1 = document.createElement("td");
                    var tdVal2 = document.createElement("td");

                    var yearKey = document.createElement("b");
                    var doc_count = document.createElement("b");

                    yearKey.innerText = buckets[i].key;
                    tdVal1.appendChild(yearKey);
                    tdVal1.style.padding = "5px";
                    doc_count.innerText = buckets[i].doc_count;
                    tdVal2.appendChild(doc_count);
                    tdVal2.style.padding = "5px";
                    tr.appendChild(tdVal1);
                    tr.appendChild(tdVal2);

                    facetTab.appendChild(tr);
                    $("#searchArea").css("cursor", "default");
                }
            }else{
                alert("No data available for given serach input")
                $("#searchArea").css("cursor", "default");
            }
        }
    });

}

/**
 * Makes the suggestion list visible only when user types in serach input field
 */
function loadResults(){
    document.getElementById("ResultsArea").style.visibility="visible";
}



// methods discarded for new data structure

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