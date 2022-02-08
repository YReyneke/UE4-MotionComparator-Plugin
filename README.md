# UE4-MotionComparator-Plugin
 The repository containing my motion comparator plugin

Click https://youtu.be/LZW51P8nFBk for a demonstration and tutorial

Usage instructions:

1. Drag this folder (MotionComparator) into the plugins folder of your project - you can create the 'Plugins' folder if you don't have one yet
	i.e. "..\UnrealProjects\YourProject\Plugins\MotionComparator"

2. Check that the plugin is enabled (Edit->Plugins->Project->MotionComparator)

2. Create a copy (Duplicate not Child) of the 'Motion_BP' actor from the plugin content folder and move this to one of your project's content folders

3. Modify the quantity (minimum of 2), and properties of the MotionPoint_BP ChildActors as desired, the following is a list of properties you can change
	NOTE: The following properties can be found by selecting one of the MotionPoint_BP components from the components list, then navigating to 'ChildActorComponent->ChildActorTemplate->Motion' in the details panel
	- Point/Movement CollisionBox->Shape->BoxExtent	- The dimensions of the collision box for this motion point as half height i.e. setting of 5 means 10cm total
	- Point/Movement CollisionBox->Transform 		- The Location/Rotation/Scale of the collision box
	- Indicator->StaticMesh							- Change this to any static mesh you desire - however all motion points must have the same static mesh (as of this version)
													  NOTE: you will also need to set a new indicator material in MotionComparatorComponent->Setup for the new mesh.
	- HasMotion										- 'True' if you want there to be a movement from this point to the next point - 'False' if the next point is in the same position
													  
4. Modify the Setup variables in Motion Comparator Component
	- Rotation Tolerance					- See tooltip. +/- means that a value of 30 would give a total variation of +30 and -30 for a total tolerance of 60 degrees of motion freedom. This sets the rotational tolerance for the entire motion (A future update may allow point specific rotational tolerance)
	- Indicator Material Motion Complete	- The material that is set on each of the indicator static mesh of each motion when that motion segment is completed
	- Next Indicator Scale					- This is used to hilight the next key point in the motion to the player - set to 1. to disable

4. In the event graph of the copied actor replace the OnMotionComplete and/or OnMotionFailed PrintString nodes with whatever behaviour you desire
	NOTE: This version requires the actors are destroyed once the comparison is complete/failed. If you want to repeat the behaviour, you should re-spawn and re-initialize the actors dynamically. Future versions may allow for repeat checks on one actor, that functionality is not yet available.

5. Place the copied actor into your level or spawn via blueprint as desired
	NOTE: The 'Initialize' (NOT 'Initialize Component') function must be called for the motion comparison to take place
	NOTE: You must call the 'initialize' function ONLY ONCE per instance to avoid undefined behaviour! This may change in future versions.

The 'Initialize' function has two parameters:
	
	ReferenceComponent (PrimitiveComponent reference object) - This component must either be or directly contain a CollisionBoxComponent
		NOTE: SceneComponent, ActorComponent etc. do not contain a CollisionBox, however StaticMesh does, this means you cannot use parent components which do not inherit a collision box where the child components are/contain collisions, only those child components will work.
		NOTE: Collsion Preset of this component must be set to OverlapAllDynamic
	
	Motions (Array of UChildActorComponent reference objects) - This must be a correctly organised array of all the MotionPoint_BP linked with this motion - minimum length of two points (or one motion segment)!
