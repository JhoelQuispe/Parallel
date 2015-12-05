import os
import sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from generator import *

def leaddm():
    w = Workflow(name="leaddm", description="""LEAD Data Mining workflow (Figure 3 in Ramakrishnan and Gannon)""")
    
    sdin1 = File(name="sd_in1.dat", size=1*KB)
    sdin2 = File(name="sd_in2.dat", size=2*MB)
    
    sdout1 = File(name="sd_out1.dat", size=4*KB)
    sdout2 = File(name="sd_out2.dat", size=1*KB)
    
    sd = Job(id="sd", namespace="leaddm", name="StormDetection", runtime=35*SECONDS, inputs=[sdin1, sdin2], outputs=[sdout1, sdout2])
    w.addJob(sd)
    
    raout1 = File(name="ra_out1.dat", size=1*KB)
    ra = Job(id="ra", namespace="leaddm", name="RemoveAttributes", runtime=66*SECONDS, inputs=[sdout2], outputs=[raout1])
    w.addJob(ra)
    
    scout1 = File(name="sc_out1.dat", size=5*KB)
    scout2 = File(name="sc_out2.dat", size=9*KB)
    sc = Job(id="sc", namespace="leaddm", name="SpatialClustering", runtime=129*SECONDS, inputs=[raout1], outputs=[scout1, scout2])
    w.addJob(sc)
    
    return w

def main(*args):
    class LEADDM(Main):
        def genworkflow(self, options):
            return leaddm()
    
    LEADDM().main(*args)

if __name__ == '__main__':
    main(*sys.argv[1:])
