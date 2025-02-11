# Social_Network_Analysis_-Recommendation_Algorithm-

This project aims to develop a recommendation algorithm for social media networks using the concept of strongly connected components (SCCs). By generating a random directed graph, we simulate a social network and identify SCCs using Tarjan's and Kosaraju's algorithms to provide mutual friend and add-back recommendations.

## Table of Contents
- [Introduction](#introduction)
- [Project Aim](#project-aim)
- [Features](#features)
- [Usage](#usage)
- [Code Overview](#code-overview)
- [Algorithms](#algorithms)
  - [Kosaraju's Algorithm](#kosarajus-algorithm)
  - [Tarjan's Algorithm](#tarjans-algorithm)
- [Output](#output)
- [Contact](#contact)


## Introduction

In social media networks, recommending friends and connections is crucial for enhancing user engagement. This project uses strongly connected components to find mutual and add-back friend recommendations. By generating a random graph, we simulate the structure of a social network and apply SCC algorithms to identify potential connections.

## Project Aim

The aim of this project is to find a recommendation algorithm for social media networks by:
- Generating a random directed graph to represent a social network.
- Identifying strongly connected components in the graph.
- Using SCCs to provide mutual friend and add-back recommendations.

## Features

- Generate a random directed graph with up to 100 vertices and 1000 edges.
- Find strongly connected components using Kosaraju's algorithm.
- Find strongly connected components using Tarjan's algorithm.
- Provide mutual friend recommendations.
- Provide add-back recommendations.

## Usage

1. **Clone the repository**:
   ```sh
   git clone https://github.com/yourusername/social_network_analysis_recommendation_algorithm.git
   cd social_network_analysis_recommendation_algorithm

2. **Build and Run**:
   ```sh
   gcc project.c -o project
   ./project.exe
  
3. **Follow the prompts** to choose between Tarjan's and Kosaraju's algorithms and view the SCCs and recommendations.

## Code Overview
The code includes the following key components:

1. Graph Structure:

   - node: Represents a vertex in the adjacency list.
   - graph: Represents the graph with adjacency list, discovery times, finish times, and components.
     
2. Stack Structure:

   - Stack: A stack implementation to assist with DFS and SCC algorithms.
     
3. Graph Functions:

   - insert_link(): Inserts a node in the adjacency list.
   - rand_graph(): Generates a random directed graph.
   - display(): Displays the adjacency list of the graph.
     
4. SCC Algorithms:

   - top_dfs(), dfs(), dfs_traversal(): Helper functions for DFS traversal.
   - topological_sort(): Performs topological sort for Kosaraju's algorithm.
   - rev_graph(): Reverses the graph.
   - kosaraju_algorithm(): Implements Kosaraju's algorithm for finding SCCs.
   - tarjan_dfs(), tarjan_algorithm(): Implements Tarjan's algorithm for finding SCCs.
     
## Algorithms

**Kosaraju's Algorithm**

Kosaraju's algorithm consists of three main steps:

   1. Perform a topological sort on the graph.
   2. Reverse the graph.
   3. Perform a DFS on the reversed graph to find SCCs.

Time Complexity Analysis:
   1. **Topological Sort:** O(V + E)
   2. **Reversing the Graph:** O(V + E)
   3. **DFS on Reversed Graph:** O(V + E)

Thus, the overall time complexity for Kosaraju's algorithm is O(V + E), where V is the number of vertices and E is the number of edges.

**Tarjan's Algorithm**

Tarjan's algorithm uses DFS to find SCCs in a single pass. It maintains discovery and low values to track the smallest discovery time reachable from each node.

Time Complexity Analysis:
   1. **DFS Traversal:** O(V + E)
   2. **Maintaining discovery and low values:** O(1) for each operation

As a result, the overall time complexity for Tarjan's algorithm is O(V + E), similar to Kosaraju's algorithm.

## Output

The program displays:

   - The adjacency list of the generated graph.
   - SCCs identified by the chosen algorithm.
   - Mutual friend recommendations.
   - Add-back recommendations.

## Contact

  - Name: Saaransh Sharma
  - GitHub: saar4nsh
