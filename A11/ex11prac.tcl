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
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]

$ns duplex-link $n0 $n2 10Mb 10ms DropTail
$ns duplex-link $n1 $n2 10Mb 10ms DropTail
$ns simplex-link $n2 $n3 0.3Mb 100ms DropTail
$ns simplex-link $n3 $n2 0.3Mb 100ms DropTail
$ns duplex-link $n3 $n4 5Mb 2ms DropTail
$ns duplex-link $n3 $n5 5Mb 2ms DropTail

$ns queue-limit $n2 $n3 10

$ns duplex-link-op $n0 $n2 orient right-down
$ns duplex-link-op $n1 $n2 orient right
$ns simplex-link-op $n2 $n3 orient down
$ns simplex-link-op $n3 $n2 orient up
$ns duplex-link-op $n3 $n4 orient left
$ns duplex-link-op $n3 $n5 orient left-down

$ns duplex-link-op $n2 $n3 queuePos 0.5

set tcp [new Agent/TCP]
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n4 $sink
$ns connect $tcp $sink

$tcp set packet_size_ 1000
$tcp set window_ 10000
$tcp set fid_ 1

set udp [new Agent/UDP]
$ns attach-agent $n1 $udp
set null [new Agent/Null]
$ns attach-agent $n5 $null
$ns connect $udp $null

$udp set fid_ 2

set ftp [new Application/FTP]
$ftp attach-agent $tcp

set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp
$cbr set packet_size_ 1000
$cbr set interval_ 0.01

$ns at 0.1 "$ftp start"
$ns at 1.0 "$cbr start"
$ns at 5.0 "$ftp stop"
$ns at 5.0 "$cbr stop"

$ns at 5.0 "finish"

$ns run