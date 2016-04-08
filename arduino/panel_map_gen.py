from pylib.panel import barmodule
from pylib.panel import panel
#Panels get defined based on their properties:
#	number	- The panel's number
#	addr	- The I2C slave address of the panel
#	flipped - Is the panel rotated 180 degrees? If so, remap the LED to maintain order
#Finally, two mapping properties get set:
#	IRmap	- Defines the order of IR sensor nodes on the panel
#	animap	- Assigns the closest LED to the IR node
#Note that the hidden led (no.50) must be the last one on each strand

default_animap = [[0,0],[0,4],[2,2],[2,6],[4,0],[4,4],[6,3],[6,6]]

#Our 4 Panel module
module_a = barmodule()
P1_1 = panel(0,9,False)
P1_1.set_IRmap([7,1,2,3,0,6,4,5])
P1_1.set_animap(default_animap)
module_a.add_panel(P1_1)

P1_2 = panel(1,8,False)
P1_2.set_IRmap([0,3,5,2,1,4,7,6])
P1_2.set_animap(default_animap)
module_a.add_panel(P1_2)

P1_3 = panel(2,10,True)
P1_3.set_IRmap([6,2,4,1,7,3,5,0])
P1_3.set_animap(default_animap)
module_a.add_panel(P1_3)

P1_4 = panel(3,15,False)
P1_4.set_IRmap([7,4,2,0,6,3,5,1])
P1_4.set_animap(default_animap)
module_a.add_panel(P1_4)

barmodule.populateGrids(module_a)
barmodule.generateC(module_a, "Pan4")

print "{0}".format(module_a.lednobyxy)
print "\n\n"
print "{0}".format(module_a.ledxbyno)
print "\n\n"
print "{0}".format(module_a.ledybyno)
print "\n\n"
print "{0}".format(module_a.IRaddresses)
print "\n\n"
print "{0}".format(module_a.IRregisters)
