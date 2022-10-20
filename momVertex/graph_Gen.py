# Python3 program to find a mother vertex in O(V+E) time
from collections import defaultdict
from random import randint
from networkx.drawing.nx_agraph import graphviz_layout
import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
# This class represents a directed graph using adjacency list
# representation


# g = nx.read_edgelist('edges.txt', create_using = nx.DiGraph(), nodetype = str)
# print(nx.info(g))
# generate a scale_free_graph, adjust (a, b, g) to generate the proper graph for test
#                          alpha, beta, gamma
g = nx.scale_free_graph(20, 0.05, 0.15, 0.8)
# alpha - Probability for adding a new node connected to an existing node chosen randomly according to the in-degree distribution.
# beta - Probability for adding an edge between two existing nodes. One existing node is chosen randomly according the in-degree 
#        distribution and the other chosen randomly according to the out-degree distribution.
# gamma - Probability for adding a new node connected to an existing node chosen randomly according to the out-degree distribution.

# break out the usual triangle of (0, 1, 2), could be omit for test
rand1 = randint(0,2)
rand2 = randint(0,2)
if (rand1 == rand2):
    rand2 = rand2 + 1 % 2
g.remove_edge(rand1, rand2)

# write graph into file
edgefile = open("edges.txt", "wb")
nx.write_edgelist(g, edgefile)
# the wirted data have form node1 node2 {}
# {} are unnecessary in here thus we have to delete them
# Reference from https://stackoverflow.com/questions/7356043/how-to-delete-specific-strings-from-a-file
infile = "messy_data_file.txt"
outfile = "cleaned_file.txt"

delete_list = ["word_1", "word_2", "word_n"]
with open(infile) as fin, open(outfile, "w+") as fout:
    for line in fin:
        for word in delete_list:
            line = line.replace(word, "")
        fout.write(line)





# used to generate a better layout for visualizing the graph
pos = nx.spring_layout(g, scale = 5)
nx.draw(g, pos, with_labels = True)
plt.show()


