var searchData=new Array();
var userEnteredtext;
var typeOfVertex;
var fromScholarlyPage;

$(function () {
    $('a[href="#search"]').on('click', function(event) {
        event.preventDefault();
        $('#search').addClass('open');
        $('#search > form > input[type="search"]').focus();
    });

    $('#search, #search button.close').on('click keyup', function(event) {
        if (event.target == this || event.target.className == 'close' || event.keyCode == 27) {
            $(this).removeClass('open');
        }
    });
});

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
    } else if (tempArray[index]._source.vType === vertexType.VENUE) {
        assignVal.value = tempArray[index]._source.VENUE_NAME;
        hiddenVal.value = vertexType.VENUE;
        document.getElementById("searchRes").focus();
    }
//}
}

function fillSuggestion() {
    var suggestionData=new Array();
    var val=document.getElementById("searchinput").value;
    console.log(val.length);
    if(val.length>0) {
        var urlSuggestion = "http://localhost:9200/dblpvertexes/_search?q=Title:\"" + val + "\"* OR AUTHOR_NAME:\"" + val +  "\"* OR JOURNAL_NAME:\""+  val +"\"* OR VENUE_NAME:\""+ val +"\"*&size=10";
        d3.json(urlSuggestion, function (error, json) {
            if (error) throw error;

            suggestionData = json.hits.hits;
            // console.log(suggestionData)
            $('#suggestionBox').html('');

            for (var i = 0; i < suggestionData.length; i++) {

                var sBox = document.getElementById("suggestionBox");
                var li = document.createElement("li");
                // var a = document.createElement("a");
                li.setAttribute("id", "aTag");
                if (suggestionData[i]._source.vType === vertexType.PAPER) {
                    //aTag = sBox.appendChild(a);

                    li.setAttribute("class", "suggestedItem");
                    li.setAttribute("id", i);
                    li.innerText = suggestionData[i]._source.Title;
                    liTag = sBox.appendChild(li);
                    liTag.onclick=function (d) {
                        if(d.path===undefined){
                            fillSearchInput( d.currentTarget.id, suggestionData);
                        }else{
                            fillSearchInput(d.path[1].getAttribute("id"), suggestionData);
                        }

                        $('#suggestionBox').html('');
                    };

                    pTag = document.createElement("p");
                    pTag.innerText = "in paper ";
                    pTag_handler = liTag.appendChild(pTag);
                    citationTag = document.createElement("bold");
                    citationTag.innerText = "Citation : ";
                    spanCitationTag = document.createElement("span");
                    if ((suggestionData[i]._source.CitationCount === undefined)||(suggestionData[i]._source.CitationCount === 0)) {
                        spanCitationTag.innerText = "NA";
                    } else {
                        spanCitationTag.innerText = suggestionData[i]._source.CitationCount;
                    }

                    pTag_handler.appendChild(citationTag);
                    pTag_handler.appendChild(spanCitationTag);



                } else if (suggestionData[i]._source.vType === vertexType.AUTHOR) {
                    // aTag = sBox.appendChild(a);
                    li.setAttribute("class", "suggestedItem");
                    li.setAttribute("id", i);
                    li.innerText = suggestionData[i]._source.AUTHOR_NAME;
                    li.onclick = function (d) {
                        if(d.path===undefined){
                            fillSearchInput(d.currentTarget.id, suggestionData);
                        }else{
                            fillSearchInput(d.path[1].getAttribute("id"), suggestionData);
                        }
                        $('#suggestionBox').html('');
                    }

                    liTag = sBox.appendChild(li);
                    pTag = document.createElement("p");
                    pTag.innerText = "in author";
                    liTag.appendChild(pTag);

                }else if(suggestionData[i]._source.vType===vertexType.PUBLICATION){
                aTag=sBox.appendChild(a);
                li.setAttribute("class","suggestedItem");
                li.setAttribute("id",i);
                li.innerText=suggestionData[i]._source.JOURNAL_NAME ;
                a.onclick=function (d) {
                    fillSearchInput(d.path[1].getAttribute("id"),suggestionData);
                    $('#suggestionBox').html('');
                }
                liTag=aTag.appendChild(li);
                pTag=document.createElement("p");
                pTag.innerText="in journal";
                liTag.appendChild(pTag);

            } else if (suggestionData[i]._source.vType === vertexType.VENUE) {
                    //aTag = sBox.appendChild(a);
                    li.setAttribute("class", "suggestedItem");
                    li.setAttribute("id", i);
                    li.innerText = suggestionData[i]._source.VENUE_NAME;
                    li.onclick = function (d) {
                        if(d.path===undefined){
                            fillSearchInput(d.currentTarget.id, suggestionData);
                        }else{
                            fillSearchInput(d.path[1].getAttribute("id"), suggestionData);
                        }
                        $('#suggestionBox').html('');
                    }
                    liTag = sBox.appendChild(li);
                    pTag = document.createElement("p");
                    pTag.innerText = "in venue";
                    liTag.appendChild(pTag);

                }
            }

        });
    }else{
        $('#suggestionBox').html('');
    }
}


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

//verifies if search field is not empty, and redirects to scholarlyResult.html
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


function searchClickedFromScholarly() {
    document.getElementById("suggestionBox").style.display="none";
   document.getElementById("content").style.cursor="wait";
     userEnteredtext=document.getElementById("searchinput").value;
     typeOfVertex=document.getElementById("hiddenInput").value;
    fromScholarlyPage=true;
    searchClicked(userEnteredtext,typeOfVertex,fromScholarlyPage);
    document.getElementById("content").style.cursor="default";
}

function searchClicked(userEnteredtext,chosenType,fromScholarly) {
    var urlSearch;

        document.getElementById("overlay").style.display="block";

    var elmnt = document.getElementById("tabArea"+getActiveTabIndex());
    elmnt.scrollLeft = 100;
    elmnt.scrollTop = 360;

    $('#search').removeClass('open');
    //Pagination code
    if(chosenType===vertexType.AUTHOR) {
        console.log(chosenType+userEnteredtext)
         urlSearch = "http://localhost:9200/dblpvertexes/_search?q=vType:" + chosenType + " AND AUTHOR_NAME:" + userEnteredtext +"&from=0&size=5";
        _LTracker.push({
            'method':'searchClicked',
            'tag': 'Search for Author',
            'url': urlSearch,
            'searchParams': {
                'chosenType': chosenType,
                'enteredText': userEnteredtext
            }
        });
    }else if((chosenType===vertexType.PAPER)){
         urlSearch = "http://localhost:9200/dblpvertexes/_search?q=vType:" + chosenType + " AND Title:" + userEnteredtext + "&from=0&size=5";
        _LTracker.push({
            'method':'searchClicked',
            'tag': 'Search for Paper',
            'url': urlSearch,
            'searchParams': {
                'chosenType': chosenType,
                'enteredText': userEnteredtext
            }
        });
    }else{
         urlSearch="http://localhost:9200/dblpvertexes/_search?q="+userEnteredtext+"&from=0&size=5";
        _LTracker.push({
            'method':'searchClicked',
            'tag': 'Search with notype',
            'url': urlSearch,
            'searchParams': {
                'chosenType': "",
                'enteredText': userEnteredtext
            }
        });
    }
    console.log(urlSearch);

    d3.json(urlSearch, function (error, json) {
        if (error)
            if (error)
                _LTracker.push({
                    'method':'searchClicked',
                    'tag': 'Error',
                    'value':error
                });

        if(json.hits.hits.length==0){
            _LTracker.push({
                'method':'searchClicked',
                'tag':'No search Result',
                'url': urlSearch,
                'searchParams': {
                    'chosenType': chosenType,
                    'enteredText': userEnteredtext
                },
                'searchResult':{
                    'totalRecods':0,
                    'timetaken':json.took
                }
            });

            searchtextfield = document.getElementById("searchText");
            searchtextfield.innerText = userEnteredtext;
            // document.getElementById("resultCount").innerText=0;
            var doc = document.getElementById("searchResults");

            var tr = document.createElement("tr");
            var td = document.createElement("td");
            boldHeader = document.createElement("b");
            boldHeader.innerText="No search results found";
            td.appendChild(boldHeader);
            tr.appendChild(td);
            doc.appendChild(tr);
            document.getElementById("overlay").style.display = "none";
        }
        else {
            var totalHits;
            var timeTaken;
            //searchData = json.hits.hits;
            searchtextfield = document.getElementById("searchText");
            //searchtextfield.setAttribute("id","hits")
            searchtextfield.innerText = userEnteredtext;

            //Pagination code
         //   $(document).ready(function () {
                $('[data-toggle="tooltip"]').tooltip();

                var $pagination = $('#pagination'),
                    totalRecords = 0,
                    records = [],
                    displayRecords = [],
                    recPerPage = 5,
                    page = 1,
                    totalPages = 0;


                totalRecords = json.hits.total;
                timeTaken=json.took;
                totalPages = Math.ceil(totalRecords / recPerPage);
                apply_pagination();

                if(fromScholarlyPage){
                    displayRecords= json.hits.hits;
                    searchData=json.hits.hits;
                    generate_table();
                }


                function generate_table() {
                    _LTracker.push({
                        'method':'generate_table',
                        'text': 'Binding result',
                        'tag':'search Result',
                        'url': urlSearch,
                        'searchParams': {
                            'chosenType': chosenType,
                            'enteredText': userEnteredtext
                        },
                        'searchResult':{
                            'totalRecods':totalRecords,
                            'timetaken':timeTaken
                        }
                    });

                    $('#searchResults').html('');

                    var doc = document.getElementById("searchResults");
                    var tr_hits=document.createElement("tr");
                    var td_hits=document.createElement("td");
                    td_hits.setAttribute("id","hits");
                    p_hits=document.createElement("p");
                    p_hits.innerText="Results found " +totalRecords+" in "+timeTaken+ " ms";
                    tdHandler=td_hits.appendChild(p_hits);

                    tr_hits.appendChild(td_hits);
                    doc.appendChild(tr_hits);

                    for (var i = 0; i < displayRecords.length; i++) {
                        var tr = document.createElement("tr");
                        var td = document.createElement("td");
                        if (displayRecords[i]._source.vType === vertexType.PAPER) {
                            a = document.createElement("a");
                            a.setAttribute("id", displayRecords[i]._id);
                            a.style.cursor="pointer";
                            a.onclick = function (d) {
                                if(d.path===undefined){
                                    poplateClickedNode(d.currentTarget.id,searchData,false);
                                }else{
                                    poplateClickedNode(d.path[1].getAttribute("id"), searchData,false);
                                }
                            };
                            a.setAttribute("data-toggle","tooltip");
                            a.setAttribute("title",displayRecords[i]._source.Title);
                            linebreak = document.createElement("br");
                            boldHeader = document.createElement("b");
                            handler_a = td.appendChild(a);
                            if(displayRecords[i]._source.Title.length>100){
                                boldHeader.innerText = displayRecords[i]._source.Title.slice(0,110)+"...";
                            }else{
                                boldHeader.innerText = displayRecords[i]._source.Title
                            }
                            handler_a.appendChild(boldHeader);

                            p = document.createElement("p");
                            handler_p = td.appendChild(p); //change to doc to keep citation and reference count out of "a" tag
                            b = document.createElement("b");
                            b.innerText = "Citation :  ";
                            span = document.createElement("span");
                            if((displayRecords[i]._source.CitationCount===undefined)||(displayRecords[i]._source.CitationCount==="0")){
                                span.innerText="NA";
                            }else{
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

                           /* b2 = document.createElement("b");
                            b2.innerText = " Type : ";
                            span2 = document.createElement("span");
                            span2.innerText = displayRecords[i]._source.articleType;
                            handler_p.appendChild(b2);
                            handler_p.appendChild(span2);*/

                            td.appendChild(linebreak);
                        } else if (displayRecords[i]._source.vType === vertexType.AUTHOR) {
                            a = document.createElement("a");
                            a.setAttribute("id", displayRecords[i]._id);
                            a.style.cursor="pointer";
                            a.onclick = function (d) {
                                if(d.path===undefined){
                                    poplateClickedNode(d.currentTarget.id,searchData,false);
                                }else{
                                    poplateClickedNode(d.path[1].getAttribute("id"), searchData,false);
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
                            b.innerText = "Total Papers :  ";
                            span = document.createElement("span");
                            span.innerText = displayRecords[i]._source.orgList;
                            handler_p.appendChild(b);
                            handler_p.appendChild(span);

                            td.appendChild(linebreak);
                        } else if (displayRecords[i]._source.vType === vertexType.ORG) {
                            a = document.createElement("a");
                            a.style.cursor="pointer";
                            a.setAttribute("id", displayRecords[i]._id);
                            a.onclick = function (d) {
                                if(d.path===undefined){
                                    poplateClickedNode(d.currentTarget.id,searchData,false);
                                }else{
                                    poplateClickedNode(d.path[1].getAttribute("id"), searchData,false);
                                }
                            };
                            linebreak = document.createElement("br");
                            boldHeader = document.createElement("b");
                            handler_a = td.appendChild(a);
                            boldHeader.innerText = displayRecords[i]._source.org;
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
                        }else if (displayRecords[i]._source.vType === vertexType.PUBLICATION) {
                            a = document.createElement("a");
                            a.setAttribute("id", displayRecords[i]._id);
                            a.style.cursor="pointer";
                            a.onclick = function (d) {
                                if(d.path===undefined){
                                    poplateClickedNode(d.currentTarget.id,searchData,false);
                                }else{
                                    poplateClickedNode(d.path[1].getAttribute("id"), searchData,false);
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
                        }else if (displayRecords[i]._source.vType === vertexType.FOS) {
                            a = document.createElement("a");
                            a.setAttribute("id", displayRecords[i]._id);
                            a.style.cursor="pointer";
                            a.onclick = function (d) {
                                if(d.path===undefined){
                                    poplateClickedNode(d.currentTarget.id,searchData,false);
                                }else{
                                    poplateClickedNode(d.path[1].getAttribute("id"), searchData,false);
                                }
                            };
                            linebreak = document.createElement("br");
                            boldHeader = document.createElement("b");
                            handler_a = td.appendChild(a);
                            boldHeader.innerText = displayRecords[i]._source.fos;
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
                        }else if (displayRecords[i]._source.vType === vertexType.VENUE) {
                            a = document.createElement("a");
                            a.setAttribute("id", displayRecords[i]._id);
                            a.style.cursor="pointer";
                            a.onclick = function (d) {
                                if(d.path===undefined){
                                    poplateClickedNode(d.currentTarget.id,searchData,false);
                                }else{
                                    poplateClickedNode(d.path[1].getAttribute("id"), searchData,false);
                                }
                            };
                            linebreak = document.createElement("br");
                            boldHeader = document.createElement("b");
                            handler_a = td.appendChild(a);
                            boldHeader.innerText = displayRecords[i]._source.VENUE_NAME;
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
                        tr.appendChild(td);
                        doc.appendChild(tr);
                    }
                   // if(!fromScholarly){
                        document.getElementById("overlay").style.display = "none";
                    //}

                }

                //calculates start and end indices and sends set of records to display
                function apply_pagination() {
                    $pagination.twbsPagination({
                        totalPages: totalPages,
                        visiblePages: 5,
                        onPageClick: function (event, page) {
                            displayRecordsIndex = Math.max(page - 1, 0) * recPerPage;
                            enteredtext=document.getElementById("searchinput").value;
                            typeOfVertex=document.getElementById("hiddenInput").value;
                            var url;
                            if(fromScholarlyPage){
                                if(typeOfVertex===vertexType.AUTHOR){
                                     url="http://localhost:9200/dblpvertexes/_search?q=vType:"+typeOfVertex+" AND AUTHOR_NAME:"+enteredtext+"&from="+displayRecordsIndex+"&size=5";
                                    console.log("table"+url)
                                }else if(typeOfVertex===vertexType.PAPER){
                                     url="http://localhost:9200/dblpvertexes/_search?q=vType:"+typeOfVertex+" AND Title:"+enteredtext+"&from="+displayRecordsIndex+"&size=5";
                                }
                                else{
                                     url="http://localhost:9200/dblpvertexes/_search?q="+enteredtext+"&from="+displayRecordsIndex+"&size=5";
                                }
                            }else{
                                if(typeOfVertex===vertexType.AUTHOR){
                                     url="http://localhost:9200/dblpvertexes/_search?q=vType:"+typeOfVertex+" AND AUTHOR_NAME:"+userEnteredtext+"&from="+displayRecordsIndex+"&size=5";
                                }else if(typeOfVertex===vertexType.PAPER){
                                 url="http://localhost:9200/dblpvertexes/_search?q=vType:"+typeOfVertex+" AND Title:"+enteredtext+"&from="+displayRecordsIndex+"&size=5";
                            }
                                else{
                                     url="http://localhost:9200/dblpvertexes/_search?q="+userEnteredtext+"&from="+displayRecordsIndex+"&size=5";

                                }
                            }

                            d3.json(url, function (error, jsonrangeResult) {
                                if (error) throw error;
                                records=jsonrangeResult.hits.hits;
                                searchData=records;
                                totalHits=jsonrangeResult.hits.total;
                                timeTaken=jsonrangeResult.took;
                                //Array.prototype.push.apply(records,rangeValues);
                                displayRecords = records;
                                generate_table();
                            });
                        }
                    });
                }
            //});
        }
    });

}

