set ns [new Simulator]

$ns color 1 Blue
$ns color 2 Red

set nf [open out.nam w]
$ns namtrace-all $nf

proc finish {} {
	global ns nf
	$ns flush-trace

	close $nf
	
	exec nam out.nam &
	exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

$ns duplex-link $n0 $n1 10Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail

$ns queue-limit $n1 $n2 10

$ns duplex-link-op $n0 $n1 orient right
$ns duplex-link-op $n1 $n2 orient right

set tcp [new Agent/TCP]
$tcp set class_ 2
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n2 $sink
$ns connect $tcp $sink

$tcp set packet_size_ 1000
$tcp set window_ 65000
$tcp set fid_ 1

set cbr [new Application/Traffic/CBR]
$cbr set packet_size_ 500
$cbr set interval_ 0.0001
$cbr attach-agent $tcp

$ns at 0.1 "$cbr start"
$ns at 4.5 "$cbr stop"

$ns at 4.5 "$ns detach-agent $n0 $tcp; $ns detach-agent $n2 $sink"

$ns at 5.0 "finish"

$ns run