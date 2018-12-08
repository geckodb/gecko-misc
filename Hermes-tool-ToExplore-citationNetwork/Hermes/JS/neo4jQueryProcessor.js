var USER_NAME="neo4j";
var PWD="123";
function clearQuery() {
    document.getElementById("queryArea").value="";
}

function submitQuery() {

    var query=document.getElementById("queryArea").value;
    var resultTextArea=document.getElementById("queryResult");

    if(query!=""){
        document.getElementById("queryContainer").style.cursor="wait";

        //creates connection to Neo4j graph db
        var driver = neo4j.v1.driver("http://"+IP_ADDRESS+":7474/browser/", neo4j.v1.auth.basic(USER_NAME, PWD),{maxTransactionRetryTime: 30000});

        // Create a session to run Cypher statements.
        var session = driver.session();
        
        _LTracker.push({
            'Database':'Neo4j',
            'method':'submitQuery',
            'Query':query,
            'UserID':sessionStorage.enteredUserID
        });

        //Run a Cypher statement, perform operation on the result in a streaming manner as records arrive
        session
            .run(query)
            .then(function (result) {
                resultTextArea.innerText=JSON.stringify(result.records);
                resultTextArea.style.color="black";
                session.close();
                document.getElementById("queryContainer").style.cursor="default";
            })
            .catch(function (error) {
                console.log(error);
                resultTextArea.innerText=error.message;
                resultTextArea.style.color="red"
            });

// This closes all used network connections.
        driver.close();
    }else{
        resultTextArea.innerText="Please enter the query!";
        resultTextArea.style.color="red"
    }

}
