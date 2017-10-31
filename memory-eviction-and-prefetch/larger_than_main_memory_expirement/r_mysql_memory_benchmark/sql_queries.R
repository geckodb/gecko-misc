query_create_region = "
CREATE TABLE region (
r_regionkey	INT	NOT NULL,
r_name	CHAR(25)	NOT NULL,
r_comment	VARCHAR(152)
) ENGINE=MEMORY  ;";

query_create_nation ="
CREATE TABLE nation (
n_nationkey	INT	NOT NULL,
n_name	CHAR(25)	NOT NULL,
n_regionkey	INT	NOT NULL,
n_comment	VARCHAR(152)
) ENGINE=MEMORY  ;"

query_create_supplier = "
CREATE TABLE supplier (
s_suppkey	INT	NOT NULL,
s_name	CHAR(25)	NOT NULL,
s_address	VARCHAR(40)	NOT NULL,
s_nationkey	INT	NOT NULL,
s_phone	CHAR(15)	NOT NULL,
s_acctbal	DECIMAL(15,2)	NOT NULL,
s_comment	VARCHAR(101)	NOT NULL
) ENGINE=MEMORY ;
"

query_create_customer ="
 CREATE TABLE customer (
c_custkey	INT	NOT NULL,
c_name	VARCHAR(25)	NOT NULL,
c_address	VARCHAR(40)	NOT NULL,
c_nationkey	INT	NOT NULL,
c_phone	CHAR(15)	NOT NULL,
c_acctbal	DECIMAL(15,2)	NOT NULL,
c_mktsegment	CHAR(10)	NOT NULL,
c_comment	VARCHAR(117)	NOT NULL
) ENGINE=MEMORY  ;";

query_create_part ="
CREATE TABLE part (
p_partkey	INT	NOT NULL,
p_name	VARCHAR(55)	NOT NULL,
p_mfgr	CHAR(25)	NOT NULL,
p_brand	CHAR(10)	NOT NULL,
p_type	VARCHAR(25)	NOT NULL,
p_size	INT	NOT NULL,
p_container	CHAR(10)	NOT NULL,
p_retailprice	DECIMAL(15,2)	NOT NULL,
p_comment	VARCHAR(23)	NOT NULL
) ENGINE=MEMORY  ;";

query_create_partsupp ="
CREATE TABLE partsupp (
ps_partkey	INT	NOT NULL,
ps_suppkey	INT	NOT NULL,
ps_availqty	INT	NOT NULL,
ps_supplycost	DECIMAL(15,2)	NOT NULL,
ps_comment	VARCHAR(199)	NOT NULL
) ENGINE=MEMORY  ;";

query_create_orders ="
CREATE TABLE orders (
o_orderkey	INT	NOT NULL, 
o_custkey	INT	NOT NULL, 
o_orderstatus	CHAR(1)	NOT NULL, 
o_totalprice	DECIMAL(15,2)	NOT NULL, 
o_orderdate	DATE	NOT NULL, 
o_orderpriority	CHAR(15)	NOT NULL, 
o_clerk	CHAR(15)	NOT NULL, 
o_shippriority	INT	NOT NULL,
o_comment	VARCHAR(79)	NOT NULL 
) ENGINE=MEMORY  ;";

query_create_lineitem ="
CREATE TABLE lineitem (
l_orderkey	INT	NOT NULL, 
l_partkey	INT	NOT NULL, 
l_suppkey	INT	NOT NULL, 
l_linenumber	INT	NOT NULL, 
l_quantity	DECIMAL(15,2)	NOT NULL, 
l_extendedprice	DECIMAL(15,2)	NOT NULL, 
l_discount	DECIMAL(15,2)	NOT NULL, 
l_tax	DECIMAL(15,2)	NOT NULL, 
l_returnflag	CHAR(1)	NOT NULL, 
l_linestatus	CHAR(1)	NOT NULL, 
l_shipdate	DATE	NOT NULL, 
l_commitdate	DATE	NOT NULL, 
l_receiptdate	DATE	NOT NULL, 
l_shipinstruct	CHAR(25)	NOT NULL, 
l_shipmode	CHAR(10)	NOT NULL, 
l_comment	VARCHAR(44)	NOT NULL 
) ENGINE=MEMORY  ; 
";



query_drop_region = "drop table region;";
query_drop_nation = "drop table nation;";
query_drop_supplier = "drop table supplier;" ; 
query_drop_partsupp =  "drop table partsupp;";
query_drop_customer =  "drop table customer;";
query_drop_orders =  "drop table orders;";
query_drop_lineitem = "drop table lineitem;";
query_drop_part =   "drop table part;";

query_turncate_region = "turncate table region;";
query_turncate_nation = "turncate table nation;";
query_turncate_supplier = "turncate table supplier;" ; 
query_turncate_partsupp =  "turncate table partsupp;";
query_turncate_customer =  "turncate table customer;";
query_turncate_orders =  "turncate table orders;";
query_turncate_lineitem = "turncate table lineitem;";
query_turncate_part =   "turncate table part;";
  
  
query_load_region = paste("LOAD DATA LOCAL INFILE '",path_to_tables,current_scale_factor,"/region.tbl' INTO TABLE region FIELDS TERMINATED BY '|';",sep="");
query_load_nation = paste("LOAD DATA LOCAL INFILE '",path_to_tables,current_scale_factor,"/nation.tbl'  INTO TABLE nation FIELDS TERMINATED BY '|';",sep="");
query_load_supplier = paste("LOAD DATA LOCAL INFILE '",path_to_tables,current_scale_factor,"/supplier.tbl'  INTO TABLE supplier FIELDS TERMINATED BY '|';",sep="");
query_load_partsupp = paste("LOAD DATA LOCAL INFILE '",path_to_tables,current_scale_factor,"/partsupp.tbl'  INTO TABLE partsupp FIELDS TERMINATED BY '|';",sep="");
query_load_customer = paste("LOAD DATA LOCAL INFILE '",path_to_tables,current_scale_factor,"/customer.tbl'  INTO TABLE customer FIELDS TERMINATED BY '|';",sep="");
query_load_orders = paste("LOAD DATA LOCAL INFILE '",path_to_tables,current_scale_factor,"/orders.tbl'  INTO TABLE orders FIELDS TERMINATED BY '|';",sep="");
query_load_lineitem = paste("LOAD DATA LOCAL INFILE '",path_to_tables,current_scale_factor,"/lineitem.tbl'  INTO TABLE lineitem FIELDS TERMINATED BY '|';",sep="");
query_load_part = paste("LOAD DATA LOCAL INFILE '",path_to_tables,current_scale_factor,"/part.tbl'  INTO TABLE part FIELDS TERMINATED BY '|';",sep="");


query_tpch_6 ="select
	sum(l_extendedprice * l_discount) as revenue
from
	lineitem
where
	l_shipdate >= date '1994-01-01'
	and l_shipdate < date '1994-01-01' + interval '1' year
	and l_discount between .06 - 0.01 and .06 + 0.01
	and l_quantity < 24;
"
