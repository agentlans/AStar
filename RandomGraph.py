import networkx as nx
import random

random.seed()

decimal_digits = 3

def random_real():
  return round(random.random(), decimal_digits)

def random_weighted_graph(n):
  '''Returns a complete graph with n vertices and random weights.'''
  G = nx.Graph()
  for i in range(n):
    for j in range(n):
      G.add_edge(i, j, weight=random_real())
  return G

def write_graph(G, file_obj):
  '''Writes adjacency matrix of weights to file_obj.'''
  n = G.number_of_nodes()
  for i in range(n):
    for j in range(n):
      file_obj.write(str(G[i][j]['weight']))
      if j != n-1:
        file_obj.write(" ")
    file_obj.write("\n")

class RandomGraph:
  def __init__(self, n):
    '''Complete graph of n nodes with random weights.'''
    self.graph = random_weighted_graph(n)
    self.source = random.randrange(n)
    self.target = random.randrange(n)
    self.n = n
  def shortest_path_length(self):
    '''Returns the length of shortest path from source to target.'''
    return nx.astar_path_length(self.graph, self.source, self.target)
  def write(self, f):
    '''Writes the graph data to file object f.'''
    f.write(str(self.n) + "\n") # dimension
    f.write(str(self.source) + " " + str(self.target) + "\n") # endpoints
    write_graph(self.graph, f) # adjacency weight matrix
    # shortest path length
    f.write(str(round(self.shortest_path_length(), decimal_digits)) + "\n")
    f.write("\n")

# Generate and save test data
filename = "test_data.txt"
with open(filename, 'w') as f:
  for i in range(1000):
    dim = random.randrange(50) + 1
    g = RandomGraph(dim)
    g.write(f)
