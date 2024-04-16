# compression 

is off by default, suggest to use snappy

# topics

Topics are partitioned, meaning a topic is spread over a number of "buckets" located on different Kafka brokers. This distributed placement of your data is very important for scalability because it allows client applications to both read and write the data from/to many brokers at the same time. When a new event is published to a topic, it is actually appended to one of the topic's partitions. Events with the same event key (e.g., a customer or vehicle ID) are written to the same partition, and Kafka guarantees that any consumer of a given topic-partition will always read that partition's events in exactly the same order as they were written.


If you sent message with key1 and after it message with the same key1, from one or more than one producer, it should end in the same partition

**WARNING** there is always only one leader for a given partition which handles reads and writes!

# producer

Kafka support the following semantics:

- at least one delivery
    * producer will try resending data if it failed to get published as it does not know if it has been commited by brokers (this is Kafka default)
- at most one delivery
    * producer will not try resending data if it failed
- exactly one delivery
     as it means by the name. this can be done with idempodent producers which would assign each producer a unique id and sequence all messages to make sure dupes are ignored on the broker side. There are also transaction capabilities in the new Kafka that allow transcation process, though consumers also need to play along
    

if acks=all and there is not enough ISR according to min.insync.replicas, producer cannot produce data and gets NotEnoughReplicas exception. But it doesn't mean that "partition is not available". consumers that subscribe this topic can still send fetch requests and partition is still available. Also other producers that have acks=1 or acks=0 can still produce messages

Batching is one of the big drivers of efficiency, and to enable batching the Kafka producer will attempt to accumulate data in memory and to send out larger batches in a single request. The batching can be configured to accumulate no more than a fixed number of messages and to wait no longer than some fixed latency bound (say 64k or 10 ms). This allows the accumulation of more bytes to send, and few larger I/O operations on the servers. This buffering is configurable and gives a mechanism to trade off a small amount of additional latency for better throughput

If you sent message with key1 and after it message with the same key1, from one or more than one producer, it should end in the same partition

## settigns

**acks=all** , requries all in-sinc replicats to confirm the message before consumers could see it

# consumer

also support the following consume guarantees:
 - consumes at least once
    consumer first consumes data and only set sets its consumer index in the log, which means if it crashes it can consume the same data few times
 - consumes at most once
     consumer would set its index in the commit log and start consuming data, but note if it crashes, any unprocesed data would be lost
 - consuems exactly once
    *idempodent* consumer??? And transactional consumer shall enable this

 Our topic is divided into a set of totally ordered partitions, each of which is consumed by exactly one consumer within each subscribing consumer group at any given time. This means that the position of a consumer in each partition is just a single integer, the offset of the next message to consume. This makes the state about what has been consumed very small, just one number for each partition. This state can be periodically checkpointed. This makes the equivalent of message acknowledgements very cheap .

 **WARNING** but it is also possible to configure consumers so that they consume all data from a a given topic!

Partition rebalances also take place in a background thread, which means you still have to handle the potential for commit failures as the consumer may no longer have the same partition assignment when the commit begins. This is unnecessary if you enable autocommit since commit failures will be ignored silently, which also implies that you have no way to rollback processing.

## settings

**enable.auto.commit**  
Default: true

Consumers acknowledge the receipt (and processing) of messages by committing the offset of the message they have read. By default, enable.auto.commit is set to true for consumer apps, which implies that the offsets are automatically committed asynchronously (for example, by a background thread in the Java consumer client) at regular intervals (defined by auto.commit.interval.ms property that defaults to 5 seconds). While this is convenient, it allows for data loss and/or duplicate message processing


**enable.auto.offset.store** 
Default: true

offsets are marked ready to store immediately prior a message being delivered to the application via consumer.Consume. Auto-commit effectively gives “at most once” delivery. 

With enable.auto.commit=true librdkafka will commit the last stored offset for each partition at regular intervals, at rebalance, and at consumer shutdown.  The offsets that are used here, are taken from an in-memory offset store. This store will be updated automatically when enable.auto.offset.store=true.  If you set enable.auto.offset.store=false you can update this in-memory offset store by yourself via rd_kafka_offsets_store().  This is preferred over disabling enable.auto.commit because you do not have to reimplement calling commit at regular intervals yourself, but can rely on the already built-in logic instead.  You have manual control about whether or not offsets are committed either way, but disabling enable.auto.commit and calling commit yourself will most likely lead to more frequent commits


# last values

Compacted topics must have records with keys in order to implement record retention.

Compaction in Kafka does not guarantee there is only one record with the same key at any one time. It is possible that there are multiple records with the same key, including the tombstone, because compaction timing is non-deterministic. Compaction is only done when the topic partition satisfies certain few conditions, such as dirty ratio, record being in inactive segment files, etc.


Compaction also allows for deletes. A message with a key and a null payload will be treated as a delete from the log. This delete marker will cause any prior message with that key to be removed (as would any new message with that key), but delete markers are special in that they will themselves be cleaned out of the log after a period of time to free up space.

## settings

**delete.retention.ms** - max time(default 24h) that key deletes are retained

# brokers

Every partition has exactly one partition leader which handles all the read/write requests of that partition.

When a partition leader shuts down for any reason (e.g a broker shuts down), one of it's in-sync partition followers becomes the new leader

Every partition follower is reading messages from the partition leader (acts like a kind of consumer) and does not serve any consumers of that partition

# kafka transactions

Second, Kafka now supports atomic writes across multiple partitions through the new transactions API. This allows a producer to send a batch of messages to multiple partitions such that either all messages in the batch are eventually visible to any consumer or none are ever visible to consumers. This feature also allows you to commit your consumer offsets in the same transaction along with the data you have processed, thereby allowing end-to-end exactly-once semantics


Firstly, enabling EOS has an effect on throughput, which can be significant. Each producer can only process transactions sequentially, so you might need multiple transactional producers, which can have a knock-on effect on the rest of the application. This is where frameworks can provide useful abstractions over multiple producer instances