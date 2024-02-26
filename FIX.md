
# restate

In the case of a corporate action resulting in the adjustment of an open GT order, the broker will send an Execution Report <8> with ExecType <150> = Restated (D) and ExecRestatementReason <378> = GT Corporate action (0) with the order's state after the corporate action adjustment. In the case of stock splits, OrderQty <38>, CumQty <14>, AvgPx <6>, and LeavesQty <151> will be adjusted to reflect the order's state in terms of current quantity (e.g. shares), not pre-split quantity (e.g. shares). See "Order State Change Matrices" for examples of GT order restatement with and without a corporate action.

# NewOrderSingle <D> message

# ExecutionReport <8> message

Each execution report contains two fields which are used to communicate both the current state of the order as understood by the broker (OrdStatus <39>) and the purpose of the message (ExecType <150>). normally ExecType would intersect with OrdStatus, but there are few exceptions like Restate,Replaced,TradeCorrect,TradeCancel, communicating that order has been cancelled / replaced - still broker may send in OrdStatus that it is "pending cancel" or "pending replace".

The ExecType <150> is used to identify the purpose of the Execution Report <8> message. To transmit a change in OrdStatus <39> for an order, the broker (sell side) should send an Execution Report <8> with the new OrdStatus <39> value in both the ExecType <150> AND the OrdStatus <39> fields to signify this message is changing the state of the order. The only exception to this rule is that when rejecting a cancel or cancel/replace request the Cancel Reject <9> message is used both to reject the request and to communicate the current OrdStatus <39>. An ExecType <150> of Pending Cancel or Pending Replace is used to indicate that a cancel or cancel/replace request is being processed. An ExecType <150> of Canceled or Replace is used to indicate that the cancel or cancel/replace request has been successfully processed.

The OrderQty <38>, CumQty <14>, LeavesQty <151>, and AvgPx <6> fields should be calculated to reflect the cumulative result of all versions of an order.

Any fills which occur and need to be communicated to the customer while an order is "pending" and waiting to achieve a new state (i.e. via a Order Cancel Replace (aka Order Modification) Request) must contain the "original" (current order prior to state change request) order parameters (i.e. ClOrdID <11>, OrderQty <38>, Price <44>, etc). These fills will cause the CumQty <14> and AvgPx <6> to be updated. An order cannot be considered replaced until it has been explicitly accepted and confirmed to have reached the replaced status via an execution report with ExecType <150> = ‘Replace', at which time the effect of the replacement (ClOrdID <11>, new quantity or limit price etc) will be seen.

The field ClOrdID <11> is provided for institutions or buy-side brokers or intermediaries to affix an identification number to an order to coincide with internal systems. The OrderID <37> field is populated with the sell-side broker-generated order number (or fund manager-generated order number for CIVs). Unlike ClOrdID <11>/OrigClOrdID <41> which requires a chaining through Cancel/Replaces and Cancels, OrderID <37> and SecondaryOrderID <198> are not required to change through changes to an order


# OrderCancelReject <9> message

The order cancel reject message is issued by the broker upon receipt of a cancel request or cancel/replace request message which cannot be honored. Requests to change price or decrease quantity are executed only when an outstanding quantity exists. Filled orders cannot be changed (i.e quantity reduced or price change. However, the broker/sellside may support increasing the order quantity on a currently filled order).

When rejecting a Cancel/Replace Request (or Cancel Request), the Cancel Reject <3> message should provide the ClOrdID <11> which was specified on the Cancel/Replace Request (or Cancel Request) message for identification, and the OrigClOrdId should be that of the last accepted order (except in the case of CxlRejReason <102> = 'Unknown Order'.

# OrderCancelRequest <F> message

The order cancel request message requests the cancelation of all of the remaining quantity of an existing order. Note that the Order Cancel/Replace Request should be used to partially cancel (reduce) an order).

The request will only be accepted if the order can successfully be pulled back from the exchange floor without executing.

A cancel request is assigned a ClOrdID <11> and is treated as a separate entity. If rejected, the ClOrdID <11> of the cancel request will be sent in the Cancel Reject <3> message, as well as the ClOrdID <11> of the actual order in the OrigClOrdID <41> field. The ClOrdID <11> assigned to the cancel request must be unique amongst the ClOrdID <11> assigned to regular orders and replacement orders.

An immediate response to this message is required. It is recommended that an ExecutionRpt with ExecType <150>=Pending Cancel be sent unless the Order Cancel Request can be immediately accepted (ExecutionRpt with ExecType <150>=Canceled) or rejected (Order Cancel Reject <3> message).

## Possible responses
Execution Report (8)
Order Cancel Reject (9)


4.0 FIX Connections and Sessions Establishing a FIX Connection
FIX connections and sessions between the Participant and server are maintained as specified in the FIX protocol.
Each Participant will use the assigned IP address and port to establish a TCP/IP session with the server. The Participant will initiate a FIX session at the start of each trading day by sending the Logon message. The Participant will identify itself using the SenderCompID (49) field. The server will validate the CompID, password and IP address of the Participant.
Once the Participant is authenticated, the server will respond with a Logon message. The SessionStatus (1409) of this message will be Session Active (0). If the Participant’s Logon message included the field NewPassword (925) and the Participant is authenticated, the SessionStatus (1409) of the Logon sent by the server will be Session Active (0).
When the Participant sends a logon with a sequence number higher than expected by the FIX Gateway, the FIX gateway will send a Resend Request and once the response/s to the Resend Request is processed by the FIX Gateway, the FIX Gateway would send a Test Request to make sure both the Participant and server is in sync before sending out any missed or new application messages.
The Participant must wait for the server’s Logon response before sending additional messages. If the Participant sends messages prior to sending the Logon message or prior to receiving the Logon response, the server will break the TCP/IP connection with the Participant without sending any message.
If a logon attempt fails because of an invalid SenderCompID, TargetCompID, IP address, invalid password or because the Participant does not have the appropriate privileges, the server will break the TCP/IP connection with the Participant without sending a Logout or Reject message.
If a logon attempt fails because of an invalid or expired password, a locked CompID or if logins are not currently permitted, the server will send a Logout message and then break the TCP/IP connection with the Participant..
If a logon attempt fails because of a session level failure (e.g. due to invalid EncryptMethod or DefaultApplVerID…etc) the inbound sequence number and the outbound sequence number both will not be incremented. In this scenario the message sequence number 1 will be sent with the Logout message.
However if a session level failure occurs due to a message sent by a Participant which contains a sequence number that is less than what is expected and the PossDupFlag (43) not being set to “Y”, then the server will send a Logout message and terminate the FIX connection. In this scenario the inbound sequence number will not be incremented but the outbound sequence number will be incremented.
If during a logon, the server receives a second connection attempt via the same TCP/IP connection while a valid FIX session is already underway for that same SenderCompID, the server will break the TCP/IP connection with the Participant without sending a Logout or Reject message. The server will not increment the next inbound message sequence number expected from the Participant as well as its own outbound message sequence number.
A protection mechanism is in place in order to protect the gateway from rapid login/logouts. If a user reaches the thresholds for rapid login/logouts, any future logins/logouts will be delayed exponentially.
45

Maintaining A FIX Session

4.2.1 Message Sequence Numbers
As outlined in the FIX protocol, the Participant and server will each maintain a separate and independent set of incoming and outgoing message sequence numbers. Sequence numbers should be initialized to 1 (one) at the start of the FIX session and be incremented throughout the session.
Monitoring sequence numbers will enable parties to identify and react to missed messages and to gracefully synchronize applications when reconnecting during a FIX session.
If any message sent by the Participant contains a sequence number that is less than what is expected and the PossDupFlag (43) is not set to “Y”, the server will send a Logout message and terminate the FIX connection. The Logout will contain the next expected sequence number in the Text (58) field.
A FIX session will not continue to the next trading day. The server will initialize its sequence numbers at the start of each day. The Participant is expected to employ the same logic.

4.2.2 Heartbeats
The Participant and server will use the Heartbeat message to exercise the communication line during periods of inactivity and to verify that the interfaces at each end are available. The heartbeat interval will be the HeartBtInt (108) specified in the Participant’s Logon message.
The server will send a Heartbeat anytime it has not transmitted a message for the heartbeat interval. The Participant is expected to employ the same logic.
As a safety mechanism, the system will not allow the user to login if the HeartBtInt is set to 0. Therefore, if the server receives a logon with HeartBtInt = 0, the user will receive a logout message with SessionStatus = 101 (Logout due to session level failure) and Text = ‘HeartBtInt should be greater than zero’.
If the server detects inactivity for a period longer than three heartbeat intervals it will send a Test Request message to force a Heartbeat from the Participant. If inactivity continues for another three heartbeat intervals, the server will send a Logout and break the TCP/IP connection with the Participant. The Participant is expected to employ similar logic if inactivity is detected on the part of the server.

4.2.3 Increasing Expected Sequence Number
The Participant or server may use the Sequence Reset message in Gap Fill mode if it wishes to increase the expected incoming sequence number of the other party. The Participant or server may also use the Sequence Reset message in Sequence Reset mode if it wishes to increase the expected incoming sequence number of the other party. The Sequence Reset mode should only be used to recover from an emergency situation. It should not be relied upon as a regular practice.
Terminating a FIX Session
The Participant is expected to terminate each FIX connection at the end of each trading day before the server shuts down. The Participant will terminate a connection by sending the Logout message. The
46
server will respond with a Logout to confirm the termination. The Participant will then break the TCP/IP connection with the server.
All open TCP/IP connections will be terminated by the server when it shuts down (a Logout will not be sent). Under exceptional circumstances the server may initiate the termination of a connection during the trading day by sending the Logout message.
If, during the exchange of Logout messages, the Participant or server detects a sequence gap, it should send a Resend Request.

Re-Establishing a FIX Session


If a FIX connection is terminated during the trading day it may be re-established via an exchange of Logon messages.
Once the Participant is authenticated, the server will respond with a Logon message. The SessionStatus (1409) of this message will be Session Active (0). If the Participant’s Logon message included the field NewPassword (925) and the Participant is authenticated, the SessionStatus (1409) of the Logon sent by the server will be Session Active (0).
When the Participant sends a logon with a sequence number higher than expected by the FIX Gateway, the FIX gateway will send a Resend Request and once the response/s to the Resend Request is processed by the FIX Gateway, the FIX Gateway would send a Test Request to make sure both the Participant and server is in sync before sending out any missed or new application messages.
The Participant must wait for the server’s Logon before sending additional messages. If additional messages are received from the Participant before the exchange of Logon messages, the TCP/IP connection with the Participant will be disconnected.
Once the FIX session is re-established successfully, the message sequence numbers will continue from the last message successfully transmitted prior to the termination.

4.4.1 Resetting Sequence Numbers

4.4.1.1 Reset Initiated by the Participant
If the Participant requires both parties to initialize (i.e. reset to 1) sequence numbers, it may use the ResetSeqNumFlag (141) field of the Logon message. The server will respond with a Logon with the ResetSeqNumFlag (141) field set to “Y” to confirm the initialization of sequence numbers.
A Participant may also manually inform Market Operations that it would like the server to initialize its sequence numbers prior to the Participant’s next login attempt.
These features are intended to help a Participant manage an emergency situation. Initializing sequence numbers on a re-login should not be relied upon as a regular practice.

4.4.1.2 Reset Initiated by the Server
The system has been designed with fault tolerance and disaster recovery technology that should ensure that the server retains its incoming and outgoing message sequence numbers for each Participant in the
47
unlikely event of an outage. However, Participants are required to support a manual request by Turquoise to initialize sequence numbers prior to the next login attempt.