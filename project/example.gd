extends Node

@onready var mExample := ExampleClass.new()


func _ready() -> void:
	mExample.print_type(mExample)


func _process(delta):
	if mExample.accumulate(delta) :
		get_tree().change_scene_to_file("res://GoHere.tscn")
	
