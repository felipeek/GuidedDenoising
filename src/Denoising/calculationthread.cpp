#include "calculationthread.h"
#include <iostream>
#include <cstdio>
#include <ctime>

CalculationThread::CalculationThread()
{
    noise_ = NULL;
    mesh_denoise_base_ = NULL;
    algorithm_name_ = "";
}

CalculationThread::~CalculationThread()
{
    if(noise_ != NULL)
        delete noise_;
    noise_ = NULL;

    if(mesh_denoise_base_ != NULL)
        delete mesh_denoise_base_;
    mesh_denoise_base_ = NULL;
}

void CalculationThread::initAlgorithm(DataManager *_data_manager, ParameterSet *_parameter_set)
{
    switch (algorithms_type_) {
    case kNoise:
        noise_ = new Noise(_data_manager, _parameter_set);
        break;
    case kBilateralMeshDenoising:
        mesh_denoise_base_ = new BilateralMeshDenoising(_data_manager, _parameter_set);
        break;
    case kNonIterativeFeaturePreservingMeshFiltering:
        mesh_denoise_base_ = new NonIterativeFeaturePreservingMeshFiltering(_data_manager, _parameter_set);
        break;
    case kFastAndEffectiveFeaturePreservingMeshDenoising:
        mesh_denoise_base_ = new FastAndEffectiveFeaturePreservingMeshDenoising(_data_manager, _parameter_set);
        break;
    case kBilateralNormalFilteringForMeshDenoising:
        mesh_denoise_base_ = new BilateralNormalFilteringForMeshDenoising(_data_manager, _parameter_set);
        break;
    case kMeshDenoisingViaL0Minimization:
        mesh_denoise_base_ = new MeshDenoisingViaL0Minimization(_data_manager, _parameter_set);
        break;
    case kGuidedMeshNormalFiltering:
        mesh_denoise_base_ = new GuidedMeshNormalFiltering(_data_manager, _parameter_set);
        break;
    default:
        break;
    }
}

void CalculationThread::run()
{
    emit(statusShowMessage("Now applying algorithm --" + algorithm_name_ + "-- ..."));
    emit(setActionAndWidget(false, false));
    if(algorithms_type_ == kNoise)
        noise_->addNoise();
    else {
        std::clock_t start;
        double duration;
        start = std::clock();
        mesh_denoise_base_->denoise();
        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        std::cout << "Filtering took " << duration << "sec. (" << algorithm_name_.toStdString().c_str() << ")." << std::endl;
    }
    emit(setActionAndWidget(true, false));
    emit(statusShowMessage("Applying algorithm --" + algorithm_name_ + "-- done."));

    emit(needToUpdateGL(false));
}
