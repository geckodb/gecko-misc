var flag=false;
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

function hideloader() {
    document.getElementById("overlay").style.display="none";
}

function searchClicked() {
    var searchData=new Array();
    document.getElementById("overlay").style.display="block";
    $('#search').removeClass('open');


    //Pagination code
    d3.json("../JSON/SearchResult.json", function (error, json) {
        if (error) throw error;

        searchData = json.hits;

        //Pagination code

        $(document).ready(function(){

            var $pagination = $('#pagination'),
                totalRecords = 0,
                records = [],
                displayRecords = [],
                recPerPage = 5,
                page = 1,
                totalPages = 0;

            records=searchData;
            console.log(records);
            totalRecords = records.length;
            totalPages = Math.ceil(totalRecords / recPerPage);
            apply_pagination();


            function generate_table() {
                $('#searchResults').html('');
                var doc=document.getElementById("searchResults");
                for (var i = 0; i < displayRecords.length; i++) {
                    var tr = document.createElement("tr");
                    var td=document.createElement("td");

                    a = document.createElement("a");
                    a.setAttribute("id",displayRecords[i].id)
                    a.onclick=function (d) {
                        poplateClickedNode(d.path[1].getAttribute("id"));
                    }
                    linebreak= document.createElement("br");
                    h5= document.createElement("b");
                    handler_a= td.appendChild(a);
                    h5.innerText=displayRecords[i].properties.title;
                    handler_a.appendChild(h5);
                    p=document.createElement("p");

                    handler_p=td.appendChild(p); //change to doc to keep it out of a tag
                    b= document.createElement("b");
                    b.innerText="Citation Count :  ";

                    span=document.createElement("span");
                    span.innerText=displayRecords[i].properties.citedby;
                    handler_p.appendChild(b);
                    handler_p.appendChild(span);

                    b1= document.createElement("b");
                    b1.innerText=" Reference Count : ";
                    span1=document.createElement("span");
                    span1.innerText=displayRecords[i].properties.reference_count;
                    handler_p.append(' ');
                    handler_p.appendChild(b1);
                    handler_p.appendChild(span1);
                    td.appendChild(linebreak);
                    tr.appendChild(td);
                    doc.appendChild(tr);
                }
                document.getElementById("overlay").style.display = "none";
            }
            //calculates start and end indices and sends set of records to display
            function apply_pagination() {
                $pagination.twbsPagination({
                    totalPages: totalPages,
                    visiblePages: 6,
                    onPageClick: function (event, page) {
                        displayRecordsIndex = Math.max(page - 1, 0) * recPerPage;
                        endRec = (displayRecordsIndex) + recPerPage;
                        displayRecords = records.slice(displayRecordsIndex, endRec);
                        generate_table();
                    }
                });
            }
        });

    });


}

