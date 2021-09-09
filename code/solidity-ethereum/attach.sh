#!/bin/bash
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin

container_id=`docker ps | grep ubuntu | cut -c1-12`

echo "entering bash of container: geth-node"
echo "container_id = ${container_id}"

docker exec -it $container_id bash

exit 0
