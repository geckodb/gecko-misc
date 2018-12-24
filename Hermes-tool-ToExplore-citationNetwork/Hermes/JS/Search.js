var searchData=new Array();
var userEnteredtext;
var typeOfVertex;
var fromScholarlyPage;
var IP_ADDRESS= "35.204.36.246";

/**
 * Assigns selected list item to the input box
 * @param index
 * @param tempArray
 */
function fillSearchInput(index,tempArray){
//if(tempArray[index]!==undefined) {
    var assignVal = document.getElementById("searchinput");
    var hiddenVal = document.getElementById("hiddenInput");
    if (tempArray[index]._source.vType === vertexType.PAPER) {
        assignVal.value = tempArray[index]._source.Title;
        hiddenVal.value = vertexType.PAPER;
        document.getElementById("searchRes").focus();
    } else if (tempArray[index]._source.vType === vertexType.AUTHOR) {
        assignVal.value = tempArray[index]._source.AUTHOR_NAME;
        hiddenVal.value = vertexType.AUTHOR;
        document.getElementById("searchRes").focus();
    }
    else if (tempArray[index]._source.vType === vertexType.PUBLICATION) {
        assignVal.value = tempArray[index]._source.JOURNAL_NAME;
        hiddenVal.value = vertexType.PUBLICATION;
        document.getElementById("searchRes").focus();
    } else if (tempArray[index]._source.vType === vertexType.TOPIC) {
        assignVal.value = tempArray[index]._source.TopicName;
        hiddenVal.value = vertexType.TOPIC;
        document.getElementById("searchRes").focus();
    }
//}
}

/**
 * shows suggestions for the text entered in search field
 */
function fillSuggestion() {
    var suggestionboxitems;
    var suggestionData=new Array();
    var val=document.getElementById("searchinput").value;
    console.log(val.length);
    if(val.length>0) {
        var urlSuggestion = "http://"+IP_ADDRESS+":9200/dblpvertexes/_search?q=Title:\"" + val + "\"* OR AUTHOR_NAME:\"" + val +  "\"* OR JOURNAL_NAME:\""+  val +"\"* OR TopicWords:\""+ val +"\"*&size=8";
        d3.json(urlSuggestion, function (error, json) {
            if (error) throw error;

            suggestionData = json.hits.hits;
            // console.log(suggestionData)
            $('#suggestionBox').html('');
            suggestionboxitems= new Set();
            for (var i = 0; i < suggestionData.length; i++) {

                var sBox = document.getElementById("suggestionBox");
                var li = document.createElement("li");
                // var a = document.createElement("a");
                li.setAttribute("id", "aTag");
                if (suggestionData[i]._source.vType === vertexType.PAPER) {
                    if (!suggestionboxitems.has(suggestionData[i]._source.Title)) {
                        suggestionboxitems.add(suggestionData[i]._source.Title);
                        li.setAttribute("class", "suggestedItem");
                        li.setAttribute("id", i);
                        li.innerText = suggestionData[i]._source.Title;
                        li.onclick = function (d) {
                            if (d.path === undefined) {
                                fillSearchInput(d.currentTarget.id, suggestionData);
                            } else {
                                fillSearchInput(d.path[1].getAttribute("id"), suggestionData);
                            }

                            $('#suggestionBox').html('');
                        };
                        liTag = sBox.appendChild(li);
                        pTag = document.createElement("p");
                        pTag.innerText = "in paper ";
                        pTag_handler = liTag.appendChild(pTag);
                        citationTag = document.createElement("bold");
                        citationTag.innerText = "Citation : ";
                        spanCitationTag = document.createElement("span");
                        if ((suggestionData[i]._source.CitationCount === undefined) || (suggestionData[i]._source.CitationCount === 0)) {
                            spanCitationTag.innerText = "NA";
                        } else {
                            spanCitationTag.innerText = suggestionData[i]._source.CitationCount;
                        }

                        pTag_handler.appendChild(citationTag);
                        pTag_handler.appendChild(spanCitationTag);
                    }
                } else if (suggestionData[i]._source.vType === vertexType.AUTHOR) {
                    if (!suggestionboxitems.has(suggestionData[i]._source.AUTHOR_ID)) {
                        suggestionboxitems.add(suggestionData[i]._source.AUTHOR_ID);

                        li.setAttribute("class", "suggestedItem");
                        li.setAttribute("id", i);
                        li.innerText = suggestionData[i]._source.AUTHOR_NAME;
                        li.onclick = function (d) {
                            if (d.path === undefined) {
                                fillSearchInput(d.currentTarget.id, suggestionData);
                            } else {
                                fillSearchInput(d.path[1].getAttribute("id"), suggestionData);
                            }
                            $('#suggestionBox').html('');
                        }

                        liTag = sBox.appendChild(li);
                        pTag = document.createElement("p");
                        pTag.innerText = "in author";
                        liTag.appendChild(pTag);
                    }
                }else if(suggestionData[i]._source.vType===vertexType.PUBLICATION) {
                    if (!suggestionboxitems.has(suggestionData[i]._source.JOURNAL_ID)) {
                        suggestionboxitems.add(suggestionData[i]._source.JOURNAL_ID);

                        li.setAttribute("class", "suggestedItem");
                        li.setAttribute("id", i);
                        li.innerText = suggestionData[i]._source.JOURNAL_NAME;
                        li.onclick = function (d) {
                            fillSearchInput(d.path[1].getAttribute("id"), suggestionData);
                            $('#suggestionBox').html('');
                        }
                        liTag = sBox.appendChild(li);
                        pTag = document.createElement("p");
                        pTag.innerText = "in journal";
                        liTag.appendChild(pTag);
                    }
                } else if (suggestionData[i]._source.vType === vertexType.TOPIC) {
                    if (!suggestionboxitems.has(suggestionData[i]._source.TopicId)) {
                        suggestionboxitems.add(suggestionData[i]._source.TopicId);
                        li.setAttribute("class", "suggestedItem");
                        li.setAttribute("id", i);
                        li.innerText = suggestionData[i]._source.TopicName;
                        li.onclick = function (d) {
                            if (d.path === undefined) {
                                fillSearchInput(d.currentTarget.id, suggestionData);
                            } else {
                                fillSearchInput(d.path[1].getAttribute("id"), suggestionData);
                            }
                            $('#suggestionBox').html('');
                        }
                        liTag = sBox.appendChild(li);
                        pTag = document.createElement("p");
                        pTag.innerText = "in topic";
                        liTag.appendChild(pTag);
                    }
                }
            }

        });
    }else{
        $('#suggestionBox').html('');
    }
}

/**
 * Submit the request from main/index page when enter is pressed
 * @param event
 */
function submitIfEnter(event) {
    if (event.keyCode === 13) {
        event.preventDefault();
        userEnteredtext=document.getElementById("searchinput").value;
        typeOfVertex=document.getElementById("hiddenInput").value;
        if(!(userEnteredtext.length>0)){

            alert("please enter value");
            event.preventDefault();
            $('#search').addClass('open');
            $('#search > form > input[type="search"]').focus();
        }else{
            window.location.href="scholarlyResult.html?value="+userEnteredtext+"&type="+typeOfVertex;
        }
    }
}

/**
 * submit the request from scholarly page when enter key is pressed, here we don't reload entire page we just update search result
 * @param event
 */
function submitIfEnter_scholarly(event) {
    if (event.keyCode === 13) {
        event.preventDefault();
        userEnteredtext=document.getElementById("searchinput").value;
        typeOfVertex=document.getElementById("hiddenInput").value;
        if(!(userEnteredtext.length>0)){
            alert("please enter value");
            event.preventDefault();
            $('#search').addClass('open');
            $('#search > form > input[type="search"]').focus();
        }else{
            //document.getElementById("suggestionBox").style.display="none";
            searchClickedFromScholarly();
        }
    }else{
        document.getElementById("suggestionBox").style.display="block";
    }
}


/**
 *verifies if search field is not empty, and redirects to scholarlyResult.html when search button is clicked
 */
$(".searchResMain").click(function(){
    userEnteredtext=document.getElementById("searchinput").value;
    typeOfVertex=document.getElementById("hiddenInput").value;
    if(!(userEnteredtext.length>0)){
        alert("please enter value");
        event.preventDefault();
        $('#search').addClass('open');
        $('#search > form > input[type="search"]').focus();
    }else{
        //alert(userEnteredtext+typeOfVertex)
        window.location.href="scholarlyResult.html?value="+userEnteredtext+"&type="+typeOfVertex,true;

    }
});


/**
 *handles sea
 * @param userEnteredtext
 * @param chosenType
 * @param fromScholarly
 */
function searchClickedFromScholarly() {
    document.getElementById("suggestionBox").style.display="none";
    document.getElementById("content").style.cursor="wait";
    userEnteredtext=document.getElementById("searchinput").value;
    typeOfVertex=document.getElementById("hiddenInput").value;
    fromScholarlyPage=true;
    searchClicked(userEnteredtext,typeOfVertex);
    document.getElementById("content").style.cursor="default";
}

/**
 *Queries for user entered texts, handles pagination and binding of results
 * @param userEnteredtext
 * @param chosenType
 * @param fromScholarly
 */
function searchClicked(userEnteredtext,chosenType) {
    var urlSearch;
    var $pagination = $('#pagination'),
        totalRecords = 0,
        records = [],
        displayRecords = [],
        recPerPage = 5,
        page = 1, //min value should be 1.
        totalPages = 1; //min value should be 1

    document.getElementById("overlay").style.display="block";
    document.getElementById("hiddenInput").value="";

    var elmnt = document.getElementById("tabArea"+getActiveTabIndex());
    elmnt.scrollLeft = 250;
    elmnt.scrollTop = 500;

    //$('#search').removeClass('open');
    //Pagination code
    if(chosenType===vertexType.AUTHOR) {
        console.log(chosenType+userEnteredtext);
        urlSearch = "http://"+IP_ADDRESS+":9200/dblpvertexes/_count?q=vType:" + chosenType + " AND AUTHOR_NAME:" + userEnteredtext;
        _LTracker.push({
            'method':'searchClicked',
            'tag': 'Search-Author',
            'Query': urlSearch,
            'UserID':sessionStorage.enteredUserID,
            'searchParams': {
                'chosenType': chosenType,
                'enteredText': userEnteredtext
            }
        });
    }else if((chosenType===vertexType.PAPER)){
        urlSearch = "http://"+IP_ADDRESS+":9200/dblpvertexes/_count?q=vType:" + chosenType + " AND Title:" + userEnteredtext+"&sort:year:desc" ;
        _LTracker.push({
            'method':'searchClicked',
            'tag': 'Search-Paper',
            'Query': urlSearch,
            'UserID':sessionStorage.enteredUserID,
            'searchParams': {
                'chosenType': chosenType,
                'enteredText': userEnteredtext
            }
        });
    }else{
        urlSearch="http://"+IP_ADDRESS+":9200/dblpvertexes/_count?q="+userEnteredtext;
        _LTracker.push({
            'method':'searchClicked',
            'tag': 'Search-noType',
            'Query': urlSearch,
            'UserID':sessionStorage.enteredUserID,
            'searchParams': {
                'chosenType': "",
                'enteredText': userEnteredtext
            }
        });
    }
    $('[data-toggle="tooltip"]').tooltip();

    $('#pagination').empty();

    $('#pagination').removeData("twbs-pagination");

    $('#pagination').unbind("page");

    $.ajax({
        dataType: "json",
        url: urlSearch,
        async: false,
        success: function (json) {

            if (json.count== 0) {
                _LTracker.push({
                    'method': 'searchClicked',
                    'tag': 'searchResult-null',
                    'Query': urlSearch,
                    'UserID':sessionStorage.enteredUserID,
                    'userid':userId,
                    'searchParams': {
                        'chosenType': chosenType,
                        'enteredText': userEnteredtext
                    },
                    'searchResult': {
                        'totalRecods': 0,
                        'timetaken': json.took
                    }
                });
                searchtextfield = document.getElementById("searchText");
                searchtextfield.innerText = userEnteredtext;

                apply_pagination(userEnteredtext,chosenType);
            }
            else {
                var totalHits;
                var timeTaken;
                searchtextfield = document.getElementById("searchText");
                searchtextfield.innerText = userEnteredtext;


                $pagination = $('#pagination'),
                    totalRecords = 0,
                    records = [],
                    displayRecords = [],
                    recPerPage = 5,
                    page = 1,
                    totalPages = 0;

                totalRecords = json.count;
                totalPages = Math.ceil(totalRecords / recPerPage);
                apply_pagination(userEnteredtext, chosenType);

            }
            //binds the result obtained for a given text
            function generate_table() {
                var count=0;
                var suggestionboxitems=new Set();
                $('#searchResults').html('');
                var doc = document.getElementById("searchResults");
                var tr_hits = document.createElement("tr");
                var td_hits = document.createElement("td");
                td_hits.setAttribute("id", "hits");
                p_hits = document.createElement("p");
                p_hits.innerText = "Results found " + totalRecords + " in " + timeTaken + " ms";
                tdHandler = td_hits.appendChild(p_hits);

                tr_hits.appendChild(td_hits);
                doc.appendChild(tr_hits);

                for (var i = 0; i < displayRecords.length; i++) {
                    if(count<5){
                    var tr = document.createElement("tr");
                    var td = document.createElement("td");
                    if (displayRecords[i]._source.vType === vertexType.PAPER) {
                        if (!suggestionboxitems.has(displayRecords[i]._source.PAPER_ID)) {
                            count++;
                            suggestionboxitems.add(displayRecords[i]._source.PAPER_ID);
                            a = document.createElement("a");
                            a.setAttribute("id", displayRecords[i]._id);
                            a.style.cursor = "pointer";
                            a.onclick = function (d) {
                                if (d.path === undefined) {
                                    poplateClickedNode(d.currentTarget.id, searchData, false);
                                } else {
                                    poplateClickedNode(d.path[1].getAttribute("id"), searchData, false);
                                }
                            };
                            a.setAttribute("data-toggle", "tooltip");
                            a.setAttribute("title", displayRecords[i]._source.Title);
                            linebreak = document.createElement("br");
                            boldHeader = document.createElement("b");
                            handler_a = td.appendChild(a);
                            if (displayRecords[i]._source.Title.length > 100) {
                                boldHeader.innerText = displayRecords[i]._source.Title.slice(0, 110) + "...";
                            } else {
                                boldHeader.innerText = displayRecords[i]._source.Title
                            }
                            handler_a.appendChild(boldHeader);

                            p = document.createElement("p");
                            handler_p = td.appendChild(p); //change to doc to keep citation and reference count out of "a" tag
                            b = document.createElement("b");
                            b.innerText = "Citation :  ";
                            span = document.createElement("span");
                            if ((displayRecords[i]._source.CitationCount === undefined) || (displayRecords[i]._source.CitationCount === "0")) {
                                span.innerText = "NA";
                            } else {
                                span.innerText = displayRecords[i]._source.CitationCount;
                            }

                            handler_p.appendChild(b);
                            handler_p.appendChild(span);

                            b1 = document.createElement("b");
                            b1.innerText = " Year of publish: ";
                            span1 = document.createElement("span");
                            span1.innerText = displayRecords[i]._source.Year;
                            handler_p.appendChild(b1);
                            handler_p.appendChild(span1);
                            td.appendChild(linebreak);
                        }
                    } else if (displayRecords[i]._source.vType === vertexType.AUTHOR) {
                        if (!suggestionboxitems.has(displayRecords[i]._source.AUTHOR_ID)) {
                            count++;
                            suggestionboxitems.add(displayRecords[i]._source.AUTHOR_ID);
                            a = document.createElement("a");
                            a.setAttribute("id", displayRecords[i]._id);
                            a.style.cursor = "pointer";
                            a.onclick = function (d) {
                                if (d.path === undefined) {
                                    poplateClickedNode(d.currentTarget.id, searchData, false);
                                } else {
                                    poplateClickedNode(d.path[1].getAttribute("id"), searchData, false);
                                }
                            }
                            linebreak = document.createElement("br");
                            boldHeader = document.createElement("b");
                            handler_a = td.appendChild(a);
                            boldHeader.innerText = displayRecords[i]._source.AUTHOR_NAME;
                            handler_a.appendChild(boldHeader);
                            p = document.createElement("p");
                            handler_p = td.appendChild(p); //change to doc to keep citation and reference count out of "a" tag
                            b = document.createElement("b");
                            b.innerText = "Papers available :  ";
                            $.ajax({
                                dataType: "json",
                                url: "http://"+IP_ADDRESS+":9200/dblprelation_authorship/_count?q=AUTHOR_ID:\"" + displayRecords[i]._source.AUTHOR_ID + "\"",
                                async: false,
                                success: function (paperCount) {
                                    span = document.createElement("span");
                                    span.innerText = paperCount.count;
                                }
                            });

                            handler_p.appendChild(b);
                            handler_p.appendChild(span);

                            td.appendChild(linebreak);
                        }
                    } else if (displayRecords[i]._source.vType === vertexType.PUBLICATION) {
                        if (!suggestionboxitems.has(displayRecords[i]._source.JOURNAL_ID)) {
                            count++;
                            suggestionboxitems.add(displayRecords[i]._source.JOURNAL_ID);
                            a = document.createElement("a");
                            a.setAttribute("id", displayRecords[i]._id);
                            a.style.cursor = "pointer";
                            a.onclick = function (d) {
                                if (d.path === undefined) {
                                    poplateClickedNode(d.currentTarget.id, searchData, false);
                                } else {
                                    poplateClickedNode(d.path[1].getAttribute("id"), searchData, false);
                                }
                            };
                            linebreak = document.createElement("br");
                            boldHeader = document.createElement("b");
                            handler_a = td.appendChild(a);
                            boldHeader.innerText = displayRecords[i]._source.JOURNAL_NAME;
                            handler_a.appendChild(boldHeader);

                            p = document.createElement("p");
                            handler_p = td.appendChild(p); //change to doc to keep citation and reference count out of "a" tag
                            b = document.createElement("b");
                            b.innerText = " ";
                            span = document.createElement("span");
                            span.innerText = " ";
                            handler_p.appendChild(b);
                            handler_p.appendChild(span);

                            td.appendChild(linebreak);
                        }
                    } else if (displayRecords[i]._source.vType === vertexType.TOPIC) {
                        if (!suggestionboxitems.has(displayRecords[i]._source.TopicId)) {
                            count++;
                            suggestionboxitems.add(displayRecords[i]._source.TopicId);
                            a = document.createElement("a");
                            a.setAttribute("id", displayRecords[i]._id);
                            a.style.cursor = "pointer";
                            a.onclick = function (d) {
                                if (d.path === undefined) {
                                    poplateClickedNode(d.currentTarget.id, searchData, false);
                                } else {
                                    poplateClickedNode(d.path[1].getAttribute("id"), searchData, false);
                                }
                            }
                            linebreak = document.createElement("br");
                            boldHeader = document.createElement("b");
                            handler_a = td.appendChild(a);
                            boldHeader.innerText = displayRecords[i]._source.TopicName;
                            handler_a.appendChild(boldHeader);
                            p = document.createElement("p");
                            handler_p = td.appendChild(p); //change to doc to keep citation and reference count out of "a" tag
                            b = document.createElement("b");
                            b.innerText = "Papers available :  ";
                            /*$.ajax({
                                dataType: "json",
                                url: "http://"+IP_ADDRESS+":9200/dblprelation_topicpaper/_count?q=Topic_Id:\""+displayRecords[i]._source.Topic_Id+"\"",
                                async: false,
                                success: function (paperCount) {
                                    span = document.createElement("span");
                                    span.innerText = paperCount.count;
                                }
                            });*/

                            handler_p.appendChild(b);
                            // handler_p.appendChild(span);

                            td.appendChild(linebreak);
                        }
                    }
                    tr.appendChild(td);
                    doc.appendChild(tr);
                    }
                }
                // if(!fromScholarly){
                document.getElementById("overlay").style.display = "none";
                //}

            }

            //calculates start and end indices and sends set of records to display
            function apply_pagination(enteredtext,typeOfVertex) {
                var url;
                $pagination.twbsPagination({
                    totalPages: totalPages,
                    visiblePages: 5,
                    onPageClick: function (event, page) {
                        displayRecordsIndex = Math.max(page - 1, 0) * recPerPage;

                        if (typeOfVertex === vertexType.AUTHOR) {
                            url = "http://"+IP_ADDRESS+":9200/dblpvertexes/_search?q=vType:" + typeOfVertex + " AND AUTHOR_NAME:" + enteredtext + "*" + "&from=" + displayRecordsIndex + "&size=10";
                            console.log("table" + url)
                        } else if (typeOfVertex === vertexType.PAPER) {
                            url = "http://"+IP_ADDRESS+":9200/dblpvertexes/_search?q=vType:" + typeOfVertex + " AND Title:" + enteredtext + "*&from=" + displayRecordsIndex + "&size=10";
                        } else if (typeOfVertex === vertexType.TOPIC) {
                            url = "http://"+IP_ADDRESS+":9200/dblpvertexes/_search?q=vType:" + typeOfVertex + " AND TopicName:" + enteredtext + "*&from=" + displayRecordsIndex + "&size=10";
                        }
                        else {
                            url = "http://"+IP_ADDRESS+":9200/dblpvertexes/_search?q=" + enteredtext + "&from=" + displayRecordsIndex + "&size=10";
                        }

                        d3.json(url, function (error, jsonrangeResult) {
                            if (error) throw error;

                            records = jsonrangeResult.hits.hits;
                            searchData = records;
                            totalHits = jsonrangeResult.hits.total;
                            timeTaken = jsonrangeResult.took;
                            displayRecords = records;

                            _LTracker.push({
                                'method': 'searchClicked',
                                'tag': 'searchResult',
                                'Query': url,
                                'UserID':sessionStorage.enteredUserID,
                                'searchParams': {
                                    'chosenType': typeOfVertex,
                                    'enteredText': enteredtext
                                },
                                'searchResult': {
                                    'totalRecods': totalHits,
                                    'timetaken': timeTaken
                                }
                            });

                            generate_table();
                        });
                    }
                });
            }
        }
    });
}

