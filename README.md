# raytracer
Functional distributed ray tracer for computer graphics

# Basic features:
- Phong illumination
- Intersection with sphere
- Intersection with plane
- Ray casting

# Advanced features:
- Recursive ray tracing
    Achieved by adding a 'depth' parameter when shooting rays. Recursively reflects rays in a slightly random direction
    until depth=0.
- Anti-aliasing:
	  Achieved by shooting more rays per pixel and averaging the colors.
- Depth of field
	  Achieved by shading each pixel with more rays that starts at random places on lens and points to the focal point.
- Extended light source, soft shadow
    Achieved by approximating point lights as spheres and using a random point on the sphere as the light's position
    during intersections.
- Texture mapping
    Achieved by adding Texture class to Material and converting 3D intersection points to UV space to map to the
    2D texture maps.
- Reflection
    Achieved by shooting another ray in the perfect specular direction given the normal and incoming ray direction.
- Refraction
	  Achieved by applying snell's law.
- Fresnel law
	  Balanced between refraction and reflection color using fresnel law.
- Glossy effect
    Achieved by distorting the direction of the reflected ray to take into account the roughness of the surface.
- Non-trivial object
    Added support for rendering cylinder objects.


# Cool demos
  ![box2](https://github.com/jordanleex13/raytracer/blob/master/demos/box/view_box_2.bmp)
  ![infmirror](https://github.com/jordanleex13/raytracer/blob/master/demos/infinity_mirror/view_infinite_mirror1.bmp)
  ![infmirrorclose](https://github.com/jordanleex13/raytracer/blob/master/demos/infinity_mirror/infinite_mirror_close_up.bmp)