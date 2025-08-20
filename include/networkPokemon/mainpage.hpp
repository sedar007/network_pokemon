/// \mainpage
///
///
/// \section intro Introduction
/// This documentation describes the Network Pokemon (Peer-to-Peer Image Sharing Application).
/// The objective of this application is to create a peer-to-peer (P2P) network for sharing Pokémon images.
/// Each node in the network plays the role of both client and server.
///
/// \section features Features
/// - Display the list of servers known by the node.
/// - Display the list of available Pokémon images on the node.
/// - Request a particular image.
///
/// \section prereq Prerequisites
/// - C++ must be installed on your system.
/// - Python or Python 3 must be installed on your system.
///
/// \section usage How to Use
/// 1. Clone this repository on your machine.
/// 2. You can now run the application in one of the following ways using a terminal or command prompt:
///    - In a terminal, launch 30 nodes and 1 node to display and download images:
///      - To launch 30 nodes with traces if necessary:
///        ```
///        python3 execute.py -n 30 -t
///        ```
///      - Then, in a second terminal, navigate to the "build" directory and launch node 31 without traces to test the functionalities:
///        ```
///        ./src/main -d ../data/peer_31/ -l nodes.txt
///        ```
///    - To launch N nodes:
///      ```
///      python3 execute.py -n N -t
///      ```
///      Where:
///      - `-n N`: the number of nodes to launch (0 < N <= 31).
///      - `-t`: to enable logs and traces.
///      Example to launch 30 nodes:
///      ```
///      python3 execute.py -n 30 -t
///      ```
