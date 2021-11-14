work_space=`pwd`
riq=${work_space}/IQ_queries/RIQ_algorithm/
rlin=${work_space}/rlin_algorithm/

iq_data=${work_space}/IQ_queries/data/

# 1x or 2x or 5x or 10x
xx=$1

# IQ Alg
cd ${riq}
./riq ${iq_data}/${xx}/path1.csv
./riq ${iq_data}/${xx}/path12.csv ${iq_data}/${xx}/path11.csv

./riq ${iq_data}/${xx}/path2.csv
./riq ${iq_data}/${xx}/path22.csv ${iq_data}/${xx}/path21.csv

./riq ${iq_data}/${xx}/path31.csv
./riq ${iq_data}/${xx}/path312.csv ${iq_data}/${xx}/path311.csv

./riq ${iq_data}/${xx}/path32.csv
./riq ${iq_data}/${xx}/path322.csv ${iq_data}/${xx}/path321.csv

./riq ${iq_data}/${xx}/path41.csv
./riq ${iq_data}/${xx}/path412.csv ${iq_data}/${xx}/path411.csv

./riq ${iq_data}/${xx}/path42.csv
./riq ${iq_data}/${xx}/path422.csv ${iq_data}/${xx}/path421.csv

./riq ${iq_data}/${xx}/path51.csv
./riq ${iq_data}/${xx}/path52.csv
./riq ${iq_data}/${xx}/path53.csv
./riq ${iq_data}/${xx}/path532.csv ${iq_data}/${xx}/path531.csv

./riq ${iq_data}/${xx}/path61.csv
./riq ${iq_data}/${xx}/path62.csv
./riq ${iq_data}/${xx}/path63.csv
./riq ${iq_data}/${xx}/path632.csv ${iq_data}/${xx}/path631.csv

# max flow alg
cd ${falg}
./falg ${iq_data}/${xx}/path1.csv
./falg ${iq_data}/${xx}/path2.csv
./falg ${iq_data}/${xx}/path31.csv
./falg ${iq_data}/${xx}/path32.csv
./falg ${iq_data}/${xx}/path41.csv
./falg ${iq_data}/${xx}/path42.csv
./falg ${iq_data}/${xx}/path51.csv
./falg ${iq_data}/${xx}/path52.csv
./falg ${iq_data}/${xx}/path53.csv
./falg ${iq_data}/${xx}/path61.csv
./falg ${iq_data}/${xx}/path62.csv
./falg ${iq_data}/${xx}/path63.csv
