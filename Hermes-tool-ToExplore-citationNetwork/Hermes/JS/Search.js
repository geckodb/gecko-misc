
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

function fillSuggestion() {
    var suggestionData=new Array();
    var val=document.getElementById("searchinput").value;
    var urlSuggestion="http://localhost:9200/janusgraph_vertexes/_search?q="+val+"&size=10";
    d3.json(urlSuggestion, function (error, json) {
        if (error) throw error;
        suggestionData=json.hits.hits;
        console.log(suggestionData)
        $('#suggestionBox').html('');
        for(let i=0;i<suggestionData.length;i++){
            var sBox=document.getElementById("suggestionBox");
            var li=document.createElement("li");
           // var a=document.createElement("a");
            if(suggestionData[i]._source.vType===vertexType.PAPER){
                li.setAttribute("id","suggestedItem");
                li.innerText=suggestionData[i]._source.title ;
                li.onclick=function (d) {
                    fillSearchInput(d)
                }
                liTag=sBox.appendChild(li)
                pTag=document.createElement("p");
                pTag.innerText="in paper"
                liTag.appendChild(pTag);

            }else if(suggestionData[i]._source.vType===vertexType.AUTHOR){
                li=document.createElement("li");
                li.setAttribute("id","suggestedItem");
                li.innerText=suggestionData[i]._source.author;
                li.onclick=function (d) {
                    fillSearchInput(d)
                }
                liTag=sBox.appendChild(li)
                pTag=document.createElement("p");
                pTag.innerText="in author"
                liTag.appendChild(pTag);
            }else if(suggestionData[i]._source.vType===vertexType.ORG){
                li=document.createElement("li");
                li.setAttribute("id","suggestedItem");
                li.innerText=suggestionData[i]._source.org ;
                li.onclick=function (d) {
                    fillSearchInput(d)
                }
                liTag=sBox.appendChild(li)
                pTag=document.createElement("p");
                pTag.innerText="in Organisation"
                liTag.appendChild(pTag);
            }else if(suggestionData[i]._source.vType===vertexType.FOS){
                li=document.createElement("li");
                li.setAttribute("id","suggestedItem");
                li.innerText=suggestionData[i]._source.fos;
                li.onclick=function (d) {
                    fillSearchInput(d)
                }
                liTag=sBox.appendChild(li)
                pTag=document.createElement("p");
                pTag.innerText="in Field of study"
                liTag.appendChild(pTag);
            }
        }
    });
}

 function fillSearchInput(data){
     alert(data);
 }

//verifies if searc field is not empty and redirects to scholarlyResult.html
$("#searchRes").click(function(){
    var userEnteredtext=document.getElementById("searchinput").value;
    if(!(userEnteredtext.length>0)){
        alert("please enter value");
        event.preventDefault();
        $('#search').addClass('open');
        $('#search > form > input[type="search"]').focus();
    }else{
        window.location.href="scholarlyResult.html?searchText="+userEnteredtext;
    }
});



function searchClicked(userEnteredtext) {
    var searchData=new Array();
    document.getElementById("overlay").style.display="block";
    $('#search').removeClass('open');
    //Pagination code
    var url="http://localhost:9200/janusgraph_vertexes/_search?q=vType:paper&q="+userEnteredtext+"&size=20"
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
        }else {
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
                totalRecords = records.length;
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
                            a.setAttribute("id", displayRecords[i]._id)
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
                            a.setAttribute("id", displayRecords[i]._id)
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
                            a.setAttribute("id", displayRecords[i]._id)
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
                            displayRecords = records.slice(displayRecordsIndex, endRec);
                            generate_table();
                        }
                    });
                }
            });
        }
    });

}

