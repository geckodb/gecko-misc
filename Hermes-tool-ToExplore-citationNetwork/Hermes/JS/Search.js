
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
    var assignVal=document.getElementById("searchinput");
    var hiddenVal=document.getElementById("hiddenInput");
    if(tempArray[index]._source.vType===vertexType.PAPER){
        assignVal.value=tempArray[index]._source.title;
        hiddenVal.value=vertexType.PAPER;
        document.getElementById("searchRes").focus();
    }else if(tempArray[index]._source.vType===vertexType.AUTHOR){
        assignVal.value=tempArray[index]._source.author;
        hiddenVal.value=vertexType.AUTHOR;
        document.getElementById("searchRes").focus();
    }else if(tempArray[index]._source.vType===vertexType.ORG){
        assignVal.value=tempArray[index]._source.org;
        hiddenVal.value=vertexType.ORG;
    }else if(tempArray[index]._source.vType===vertexType.FOS){
        assignVal.value=tempArray[index]._source.fos;
        hiddenVal.value=vertexType.FOS;
        document.getElementById("searchRes").focus();
    }else if(tempArray[index]._source.vType===vertexType.PUBLICATION){
        assignVal.value=tempArray[index]._source.publisher;
        hiddenVal.value=vertexType.PUBLICATION;
        document.getElementById("searchRes").focus();
    }
}

function fillSuggestion() {

    var suggestionData=new Array();
    var val=document.getElementById("searchinput").value;
    var urlSuggestion="http://localhost:9200/janusgraph_vertexes/_search?q="+val+"&size=10";
    d3.json(urlSuggestion, function (error, json) {
        if (error) throw error;

        suggestionData=json.hits.hits;
        // console.log(suggestionData)
        $('#suggestionBox').html('');

        for(var i=0;i<suggestionData.length;i++){

            var sBox=document.getElementById("suggestionBox");
            var li=document.createElement("li");
            var a=document.createElement("a");
            a.setAttribute("id","aTag");
            if(suggestionData[i]._source.vType===vertexType.PAPER){
                aTag=sBox.appendChild(a);

                li.setAttribute("class","suggestedItem");
                li.setAttribute("id",i);
                li.innerText=suggestionData[i]._source.title ;
                a.onclick=function (d) {
                    fillSearchInput(d.path[1].getAttribute("id"),suggestionData);
                    $('#suggestionBox').html('');
                }
                liTag=aTag.appendChild(li);
                pTag=document.createElement("p");
                pTag.innerText="in paper ";
                pTag_handler=liTag.appendChild(pTag);
                citationTag=document.createElement("bold");
                citationTag.innerText="Citation : ";
                spanCitationTag=document.createElement("span");
                spanCitationTag.innerText=suggestionData[i]._source.citationCount;

                pTag_handler.appendChild(citationTag);
                pTag_handler.appendChild(spanCitationTag);

            }else if(suggestionData[i]._source.vType===vertexType.AUTHOR){
                aTag=sBox.appendChild(a);
                li.setAttribute("class","suggestedItem");
                li.setAttribute("id",i);
                li.innerText=suggestionData[i]._source.author;
                a.onclick=function (d) {
                    fillSearchInput(d.path[1].getAttribute("id"),suggestionData);
                    $('#suggestionBox').html('');
                }
                liTag=aTag.appendChild(li)
                pTag=document.createElement("p");
                pTag.innerText="in author"
                liTag.appendChild(pTag);

            }else if(suggestionData[i]._source.vType===vertexType.ORG){
                aTag=sBox.appendChild(a);
                li.setAttribute("class","suggestedItem");
                li.setAttribute("id",i);
                li.innerText=suggestionData[i]._source.org ;
                a.onclick=function (d) {
                    fillSearchInput(d.path[1].getAttribute("id"),suggestionData);
                    $('#suggestionBox').html('');
                }
                liTag=aTag.appendChild(li)
                pTag=document.createElement("p");
                pTag.innerText="in Organisation"
                liTag.appendChild(pTag);

            }else if(suggestionData[i]._source.vType===vertexType.FOS){
                aTag=sBox.appendChild(a);
                li.setAttribute("class","suggestedItem");
                li.setAttribute("id",i);
                li.innerText=suggestionData[i]._source.fos;
                a.onclick=function (d) {
                    fillSearchInput(d.path[1].getAttribute("id"),suggestionData);
                    $('#suggestionBox').html('');
                }
                liTag=aTag.appendChild(li)
                pTag=document.createElement("p");
                pTag.innerText="in Field of study"
                liTag.appendChild(pTag);

            }else if(suggestionData[i]._source.vType===vertexType.PUBLICATION){
                aTag=sBox.appendChild(a);
                li.setAttribute("class","suggestedItem");
                li.setAttribute("id",i);
                li.innerText=suggestionData[i]._source.publisher;
                a.onclick=function (d) {
                    fillSearchInput(d.path[1].getAttribute("id"),suggestionData);
                    $('#suggestionBox').html('');
                }
                liTag=aTag.appendChild(li)
                pTag=document.createElement("p");
                pTag.innerText="in publication"
                liTag.appendChild(pTag);
            }
        }
    });

}


//verifies if search field is not empty, and redirects to scholarlyResult.html
$("#searchRes").click(function(){
    var userEnteredtext=document.getElementById("searchinput").value;
    var typeOfVertex=document.getElementById("hiddenInput").value;
    if(!(userEnteredtext.length>0)){

        alert("please enter value");
        event.preventDefault();
        $('#search').addClass('open');
        $('#search > form > input[type="search"]').focus();
    }else{
        window.location.href="scholarlyResult.html?value="+userEnteredtext+"&type="+typeOfVertex;
    }
});



function searchClicked(userEnteredtext,chosenType) {
    var searchData=new Array();
    document.getElementById("overlay").style.display="block";
    $('#search').removeClass('open');
    //Pagination code
    if(chosenType!=""){
        var url="http://localhost:9200/janusgraph_vertexes/_search?q=vType:"+chosenType+" AND "+userEnteredtext+"&from=0&size=5";
        //http://localhost:9200/janusgraph_vertexes/_search?q=vType:paper AND relational data&from=50&size=500
    }else{
        var url="http://localhost:9200/janusgraph_vertexes/_search?q="+userEnteredtext+"&from=0&size=5";

    }

    d3.json(url, function (error, json) {
        if (error) throw error;
        if(json.hits.hits.length==0){
            searchtextfield = document.getElementById("searchText");
            searchtextfield.innerText = userEnteredtext;
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
            searchData = json.hits.hits;
            searchtextfield = document.getElementById("searchText");
            searchtextfield.innerText = userEnteredtext;
            console.log(searchData)
            //Pagination code
            $(document).ready(function () {
                var $pagination = $('#pagination'),
                    totalRecords = 0,
                    records = [],
                    displayRecords = [],
                    recPerPage = 5,
                    page = 1,
                    totalPages = 0;

                records = searchData;
                totalRecords = json.hits.total;
                totalPages = Math.ceil(totalRecords / recPerPage);
                apply_pagination();


                function generate_table() {
                    $('#searchResults').html('');

                    var doc = document.getElementById("searchResults");
                    for (var i = 0; i < displayRecords.length; i++) {
                        var tr = document.createElement("tr");
                        var td = document.createElement("td");
                        if (displayRecords[i]._source.vType === vertexType.PAPER) {
                            a = document.createElement("a");
                            a.setAttribute("id", displayRecords[i]._id);
                            a.style.cursor="pointer";
                            a.onclick = function (d) {
                                poplateClickedNode(d.path[1].getAttribute("id"), searchData);
                            }
                            linebreak = document.createElement("br");
                            boldHeader = document.createElement("b");
                            handler_a = td.appendChild(a);
                            boldHeader.innerText = displayRecords[i]._source.title;
                            handler_a.appendChild(boldHeader);

                            p = document.createElement("p");
                            handler_p = td.appendChild(p); //change to doc to keep citation and reference count out of "a" tag
                            b = document.createElement("b");
                            b.innerText = "Citation Count :  ";
                            span = document.createElement("span");
                            span.innerText = displayRecords[i]._source.citationCount;
                            handler_p.appendChild(b);
                            handler_p.appendChild(span);

                            b1 = document.createElement("b");
                            b1.innerText = " Year of publish : ";
                            span1 = document.createElement("span");
                            span1.innerText = displayRecords[i]._source.year;
                            handler_p.appendChild(b1);
                            handler_p.appendChild(span1);
                            td.appendChild(linebreak);
                        } else if (displayRecords[i]._source.vType === vertexType.AUTHOR) {
                            a = document.createElement("a");
                            a.setAttribute("id", displayRecords[i]._id);
                            a.onclick = function (d) {
                                poplateClickedNode(d.path[1].getAttribute("id"), searchData);
                            }
                            linebreak = document.createElement("br");
                            boldHeader = document.createElement("b");
                            handler_a = td.appendChild(a);
                            boldHeader.innerText = displayRecords[i]._source.author;
                            handler_a.appendChild(boldHeader);

                            p = document.createElement("p");
                            handler_p = td.appendChild(p); //change to doc to keep citation and reference count out of "a" tag
                            b = document.createElement("b");
                            b.innerText = "Affiliation :  ";
                            span = document.createElement("span");
                            span.innerText = displayRecords[i]._source.orgList;
                            handler_p.appendChild(b);
                            handler_p.appendChild(span);

                            td.appendChild(linebreak);
                        } else if (displayRecords[i]._source.vType === vertexType.ORG) {
                            a = document.createElement("a");
                            a.setAttribute("id", displayRecords[i]._id);
                            a.onclick = function (d) {
                                poplateClickedNode(d.path[1].getAttribute("id"), searchData);
                            }
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
                            a.onclick = function (d) {
                                poplateClickedNode(d.path[1].getAttribute("id"), searchData);
                            }
                            linebreak = document.createElement("br");
                            boldHeader = document.createElement("b");
                            handler_a = td.appendChild(a);
                            boldHeader.innerText = displayRecords[i]._source.publisher;
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
                            a.onclick = function (d) {
                                poplateClickedNode(d.path[1].getAttribute("id"), searchData);
                            }
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
                            a.onclick = function (d) {
                                poplateClickedNode(d.path[1].getAttribute("id"), searchData);
                            }
                            linebreak = document.createElement("br");
                            boldHeader = document.createElement("b");
                            handler_a = td.appendChild(a);
                            boldHeader.innerText = displayRecords[i]._source.venue;
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

                    document.getElementById("overlay").style.display = "none";
                }

                //calculates start and end indices and sends set of records to display
                function apply_pagination() {
                    $pagination.twbsPagination({
                        totalPages: totalPages,
                        visiblePages: 5,
                        onPageClick: function (event, page) {
                            displayRecordsIndex = Math.max(page - 1, 0) * recPerPage;
                            endRec = (displayRecordsIndex) + recPerPage;

                            if(displayRecordsIndex==0){
                                displayRecords = records.slice(displayRecordsIndex, endRec);
                                generate_table();
                            }else if(displayRecordsIndex>0){
                                var rangeValues=new Array();
                                if(chosenType!=""){
                                    var url="http://localhost:9200/janusgraph_vertexes/_search?q=vType:"+chosenType+" AND "+userEnteredtext+"&from="+displayRecordsIndex+"&size=5";
                                }else{
                                    var url="http://localhost:9200/janusgraph_vertexes/_search?q="+userEnteredtext+"&from="+displayRecordsIndex+"&size=5";

                                }
                                d3.json(url, function (error, jsonrangeResult) {
                                    if (error) throw error;
                                    rangeValues=jsonrangeResult.hits.hits;
                                   Array.prototype.push.apply(records,rangeValues);
                                    displayRecords = records.slice(displayRecordsIndex, endRec);
                                    generate_table();
                                });

                            }
                        }
                    });
                }
            });
        }
    });

}

