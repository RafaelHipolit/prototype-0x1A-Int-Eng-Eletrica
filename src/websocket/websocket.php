<?php
//fonte: https://www.youtube.com/watch?v=eFhOlZTFyr4
require 'vendor/autoload.php';

use Ratchet\MessageComponentInterface;
use Ratchet\ConnectionInterface;
use Ratchet\Server\IoServer;
use Ratchet\Http\HttpServer;
use Ratchet\WebSocket\WsServer;

define("IP_ADDRESS", "192.168.1.12");

class MyWebSocketServer implements MessageComponentInterface {
    public $clients;
    private $connectedClients;

    public function __construct()
    {
        $this->clients = new \SplObjectStorage;
        $this->connectedClients = [];
    }

    public function onOpen(ConnectionInterface $conn) //chamado quando um novo cliente tenta entrar no server
    {
        $this->clients->attach($conn); //junta o client ao server
        $this->connectedClients[$conn->resourceId] = $conn;
        echo "New connection {$conn->resourceId} \n";
        $conn->send("Welcome to the server\n");
    }

    public function onMessage(ConnectionInterface $from, $msg) //chamado quando uma mensagem eh recebida
    {
        echo $msg . "\n";
        foreach ($this->connectedClients as $c){
            $c->send($msg);
        }
    }

    public function onClose(ConnectionInterface $conn) //chamado quando um cliente sai do server
    {
        echo "Connection closed {$conn->resourceId} \n";
        $conn->close();
    }

    public function onError(ConnectionInterface $conn, Exception $e)
    {
        echo "Erro: " . $e->getMessage() . "\n";
        $conn->close();
    }
}

/*
$app = new Ratchet\App(IP_ADDRESS, 81, "0.0.0.0"); //ip address, port,?
$app->route('/', new myWebSocket, array('*')); // ws://[ip address]:[port]/
$app->run();
*/

$server = IoServer::factory(
    new HttpServer(
        new WsServer(
            new MyWebSocketServer()
        )
    ),
    8080
);

$server->run();


?>