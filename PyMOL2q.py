#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Copyright 2020 University of Groningen
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


"""
Program and Pymol module for visualizing q/tsi files

@author: Tsjerk A. Wassenaar
"""


from pymol import cmd
from pymol.cgo import *

import numpy as np


def read_section(stream, dtype=float):
    """
    Read a section from a (q/tsi) file as a numpy array.

    The q/tsi files have array sections, preceded by the number of entries.
    This function reads one such section from an open stream.

    Parameters
    ----------
    stream: an open file stream
    dtype: type for array

    Returns
    -------
    Numpy array
    """
    n = int(stream.readline().split()[-1])
    if n:
        return np.loadtxt([stream.readline() for _ in range(n)], dtype=dtype)
    return


def readq(filename):
    """
    Read vertices and triangles from a q-file.

    Parameters
    ----------
    filename: str

    Returns
    -------
    tuple
        vertices (numpy array)
        triangles (numpy array)
        box (str)
    """
    with open(filename) as q:
        box = q.readline()
        vertices = read_section(q)[:, 1:]
        triangles = read_section(q, dtype=int)[:, 1:]

    return vertices, triangles, box


def readtsi(filename):
    """
    Generator for reading sets of vertices and triangles from a tsi file.

    Parameters
    ----------
    filename: str
 
    Returns
    -------
    generator object that yields tuples of
        timestep: float
        vertices: numpy array
        triangles: numpy array
        inclusions: numpy array
        box: str
    """
    with open(filename) as tsi:
        timestep = tsi.readline()
        while timestep:
            box = tsi.readline()
            vertices = read_section(tsi)[:, 1:]
            triangles = read_section(tsi, dtype=int)[:, 1:]
            inclusions = read_section(tsi)
            yield float(timestep), vertices, triangles, inclusions, box
            timestep = tsi.readline()
            

def triangles2cgo(V, T):
    """
    Convert arrays of vertices and triangles into (PyMol) CGO objects.

    Parameters
    ----------
    V: numpy array (vertices)
    T: numpy array (triangles, dtype: int)

    Returns
    -------
    tuple
        points: CGO list
        mesh: CGO list
    """
    # Remove points listed for not drawing (connected over PBC)
    if T.shape[1] == 4:
        T = T[T[:, 3] != 0, :3]

    if V.shape[1] == 4:
        V = V[:, :-1]
        
    # Construct CGO objects
    mesh = [BEGIN, LINES, COLOR, 1.0, 1.0, 1.0]
    for i,j,k in T[:, :4]:
        mesh.extend([u for a in [i,j,j,k,k,i] for u in [VERTEX] + list(V[a])])
    mesh.append(END)

    points = [u for x,y,z in V[:,:3] for u in [SPHERE, x, y, z, 1]]

    return points, mesh


def writecgo(filename, V, T, mesh="mesh", points=None):
    p, m = q2cgo(V, T)
    raise NotImplemented

    
def loadq(filename, mesh="mesh", points=None):
    """
    Read points/mesh from q/tsi file and draw objects
    """
    if filename.endswith(".tsi"):
        it0 = readtsi(filename)
    else:
        V, T, box = readq(filename)
        it0 = [(0, V, T, [], box)]

    
    for timestep, vertices, triangles, inclusions, box in it0:
        p, m = triangles2cgo(vertices, triangles)

        if points is not None:
            cmd.load_cgo(p, points)

        if mesh is not None:
            cmd.load_cgo(m, mesh)

            
if __name__ == "__main__":
    sys.exit()

if __name__ == "pymol":
    from pymol import cmd
    cmd.extend("loadq", loadq)
