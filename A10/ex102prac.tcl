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

set n(0) [$ns node]
set n(1) [$ns node]
set n(2) [$ns node]
set n(3) [$ns node]

for {set i 0} {$i < 3} {incr i} {
	$ns duplex-link $n($i) $n([expr $i+1]) 1Mb 10ms DropTail
}
$ns duplex-link $n(0) $n(3) 1Mb 10ms DropTail
$ns duplex-link $n(1) $n(3) 1Mb 10ms DropTail

$ns duplex-link-op $n(0) $n(1) orient right
$ns duplex-link-op $n(1) $n(2) orient down
$ns duplex-link-op $n(2) $n(3) orient left
$ns duplex-link-op $n(3) $n(0) orient up
$ns duplex-link-op $n(1) $n(3) orient left-down

set tcp1 [new Agent/TCP]
$ns attach-agent $n(0) $tcp1
set sink [new Agent/TCPSink]
$ns attach-agent $n(3) $sink
$ns connect $tcp1 $sink
$tcp1 set fid_ 1

set tcp2 [new Agent/TCP]
$ns attach-agent $n(1) $tcp2
set sink [new Agent/TCPSink]
$ns attach-agent $n(3) $sink
$ns connect $tcp2 $sink
$tcp2 set fid_ 2

set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $tcp1
$cbr1 set type_ CBR
$cbr1 set packet_size_ 1000
$cbr1 set rate_ 1mb
$cbr1 set random_ false

set cbr2 [new Application/Traffic/CBR]
$cbr2 attach-agent $tcp2
$cbr2 set type_ CBR
$cbr2 set packet_size_ 1000
$cbr2 set rate_ 1mb
$cbr2 set random_ false

$ns rtproto DV

$ns rtmodel-at 1.0 down $n(1) $n(3)
$ns rtmodel-at 2.0 up $n(1) $n(3)

$ns at 0.1 "$cbr1 start"
$ns at 0.2 "$cbr2 start"
$ns at 4.5 "$ns detach-agent $n(0) $tcp1; $ns detach-agent $n(3) $sink; $ns detach-agent $n(1) $tcp2"
$ns at 5.0 "finish"

$ns run

