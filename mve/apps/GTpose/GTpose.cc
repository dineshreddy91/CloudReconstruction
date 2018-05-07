/*
 * Copyright (C) 2015, Simon Fuhrmann
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

#include "util/system.h"
#include "util/timer.h"
#include "util/arguments.h"
#include "util/file_system.h"
#include "util/tokenizer.h"
#include "mve/scene.h"
#include "mve/bundle.h"
#include "mve/bundle_io.h"
#include "mve/image.h"
#include "mve/image_tools.h"

#define RAND_SEED_MATCHING 0
#define RAND_SEED_SFM 0


mve::Bundle::Ptr
load_bundle_from (std::string const& scene_directory)
{
    std::string file_path = util::fs::join_path(scene_directory, "synth_0.out");
	std::cout<<file_path<<std::endl;
    return mve::load_mve_bundle(file_path);
}

int main (int argc, char** argv)
{	
	std::cout<<argv[1]<<std::endl;
    mve::Scene::Ptr scene;
    try
    {
        scene = mve::Scene::create(argv[1]);
    }
    catch (std::exception& e)
    {
        std::cerr << "Error loading scene: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
}


	std::cout<<argv[1]<<std::endl;
    mve::Bundle::Ptr bundle = load_bundle_from(argv[1]);
	std::cout<<"asasjakjsghaskjh"<<std::endl;
	//mve::load_bundle_from(argv[1]);
	 mve::Bundle::Cameras const& bundle_cams = bundle->get_cameras();
	mve::Scene::ViewList const& views = scene->get_views();
	    if (bundle_cams.size() != views.size())
    {
        std::cerr << "Error: Invalid number of cameras!" << std::endl;
        std::exit(EXIT_FAILURE);
}
    for (std::size_t i = 0; i < bundle_cams.size(); ++i)
    {
        mve::View::Ptr view = views[i];
        mve::CameraInfo const& cam = bundle_cams[i];
        if (view == nullptr)
            continue;
        if (view->get_camera().flen == 0.0f && cam.flen == 0.0f)
            continue;

        view->set_camera(cam);

        std::cout << "Saving view " << view->get_directory() << std::endl;
        view->save_view();
        view->cache_cleanup();
}

}
