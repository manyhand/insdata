work_space=`pwd`
riq=${work_space}/IQ_queries/RIQ_algorithm/
rlin=${work_space}/rlin_algorithm/

cd $riq
make clean
make

cd $falg
make clean
make
