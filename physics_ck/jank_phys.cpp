if(is_colliding(gm, current_boat, boat_to_compare, 0)){
                    // If this passes, do more granular collision.  This is barebones so I'll just treat this as a collision.
                    //I am just going to reverse the accelerations for both ships to see if that fixes it.  I can do something more special later.
                    if(verbose)
                        printf("Collided\n");
                    vec2 combined_dir = vec2(gm.merchants[current_boat].velocity.x + gm.merchants[boat_to_compare].velocity.x, 
                                            gm.merchants[current_boat].velocity.y + gm.merchants[boat_to_compare].velocity.y);
                    float combined_magnitude = gm.merchants[current_boat].velocity.Length() + gm.merchants[boat_to_compare].velocity.Length();
                    vec2 combined_dir_ortho = vec2(-combined_dir.y, combined_dir.x);
 
                    // //Compute resulting direction for current_boat
                    // float current_boat_resulting_directionX = gm.merchants[current_boat].velocity.x - 2 * 
                    //                                         dot_product(gm.merchants[current_boat].velocity, combined_dir_ortho.Normalize()) * 
                    //                                         combined_dir_ortho.Normalize().x;
                    // float current_boat_resulting_directionY = gm.merchants[current_boat].velocity.y - 2 * 
                    //                                         dot_product(gm.merchants[current_boat].velocity, combined_dir_ortho.Normalize()) * 
                    //                                         combined_dir_ortho.Normalize().y;
                    // vec2 resulting_current_boat_vel_dir = vec2(current_boat_resulting_directionX, current_boat_resulting_directionY);
                    // //Same for other boat
                    // float boat_to_compare_resulting_directionX = gm.merchants[boat_to_compare].velocity.x - 2 * 
                    //                                         dot_product(gm.merchants[boat_to_compare].velocity, combined_dir_ortho.Normalize()) * 
                    //                                         combined_dir_ortho.Normalize().x;
                    // float boat_to_compare_resulting_directionY = gm.merchants[boat_to_compare].velocity.y - 2 * 
                    //                                         dot_product(gm.merchants[boat_to_compare].velocity, combined_dir_ortho.Normalize()) * 
                    //                                         combined_dir_ortho.Normalize().y;
                    // vec2 resulting_boat_to_compare_vel_dir = vec2(boat_to_compare_resulting_directionX, boat_to_compare_resulting_directionY);
                    // //Normalizing both and then giving them each half the velocity of the total
                    // resulting_boat_to_compare_vel_dir = resulting_boat_to_compare_vel_dir.Normalize();
                    // resulting_boat_to_compare_vel_dir.x *= combined_magnitude / 2;
                    // resulting_boat_to_compare_vel_dir.y *= combined_magnitude / 2;
                    // resulting_current_boat_vel_dir = resulting_current_boat_vel_dir.Normalize();
                    // resulting_current_boat_vel_dir.x *= combined_magnitude / 2;
                    // resulting_current_boat_vel_dir.y *= combined_magnitude / 2;

                    vec2 resulting_current_boat_vel_dir = vec2(gm.merchants[current_boat].velocity.Normalize().x * combined_magnitude/4.0,
                                                                gm.merchants[current_boat].velocity.Normalize().y * combined_magnitude/4.0);
                    vec2 resulting_boat_to_compare_vel_dir = vec2(gm.merchants[current_boat].velocity.Normalize().x * combined_magnitude * .75,
                                                                gm.merchants[current_boat].velocity.Normalize().y * combined_magnitude * .75);

                    gm.merchants[current_boat].velocity = resulting_current_boat_vel_dir;
                    gm.merchants[boat_to_compare].velocity = resulting_boat_to_compare_vel_dir;

                    // gm.merchants[current_boat].orientation = atan2(gm.merchants[current_boat].velocity.y, gm.merchants[current_boat].velocity.x) * 1/pioveroneeighty;
                    // gm.merchants[boat_to_compare].orientation = atan2(gm.merchants[boat_to_compare].velocity.y, gm.merchants[boat_to_compare].velocity.x) * 1/pioveroneeighty;

                    //Keep adding the velocity to the position until they don't collide
                    while((is_colliding(gm, current_boat, boat_to_compare,0)) || (is_colliding(gm, boat_to_compare, current_boat,0))){
                        //Manually setting new positions

                        vec2 vector_for_curr_away_from_comp = vec2(gm.merchants[current_boat].coord_pos.x - gm.merchants[boat_to_compare].coord_pos.x,
                                                                    gm.merchants[current_boat].coord_pos.y - gm.merchants[boat_to_compare].coord_pos.y);
                        vec2 vector_for_comp_away_from_curr = vec2(gm.merchants[boat_to_compare].coord_pos.x - gm.merchants[current_boat].coord_pos.x,
                                                                    gm.merchants[boat_to_compare].coord_pos.y - gm.merchants[current_boat].coord_pos.y);

                        gm.merchants[current_boat].coord_pos.x += vector_for_curr_away_from_comp.Normalize().x * engineMultiplier;
                        gm.merchants[current_boat].coord_pos.y += vector_for_curr_away_from_comp.Normalize().y * engineMultiplier;

                        gm.merchants[boat_to_compare].coord_pos.x += vector_for_comp_away_from_curr.Normalize().x * engineMultiplier;
                        gm.merchants[boat_to_compare].coord_pos.y += vector_for_comp_away_from_curr.Normalize().y * engineMultiplier;

                        //Moving the merchant to the appropriate tile
                        vec2 newLoc_curr = vec2(gm.merchants[current_boat].coord_pos.x,gm.merchants[current_boat].coord_pos.y);
                        gm.merchants[current_boat].position = convert_coord_tile(gm, newLoc_curr);

                        vec2 newLoc_comp = vec2(gm.merchants[boat_to_compare].coord_pos.x,gm.merchants[boat_to_compare].coord_pos.y);
                        gm.merchants[boat_to_compare].position = convert_coord_tile(gm, newLoc_comp);
                    }
                }