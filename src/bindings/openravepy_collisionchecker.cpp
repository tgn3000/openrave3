// -*- coding: utf-8 -*-
// Copyright (C) 2006-2013 Rosen Diankov <rosen.diankov@gmail.com>
//
// This file is part of OpenRAVE.
// OpenRAVE is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#define NO_IMPORT_ARRAY
#include "openravepy_int.h"
#include "openravepy_collisionreport.h"
#include "openravepy_collisioncheckerbase.h"
#include "openravepy_kinbody.h"
#include "openravepy_environment.h"

namespace openravepy {

CollisionCheckerBasePtr GetCollisionChecker(PyCollisionCheckerBasePtr pyCollisionChecker)
{
    return !pyCollisionChecker ? CollisionCheckerBasePtr() : pyCollisionChecker->GetCollisionChecker();
}

PyInterfaceBasePtr toPyCollisionChecker(CollisionCheckerBasePtr pCollisionChecker, PyEnvironmentBasePtr pyenv)
{
    return !pCollisionChecker ? PyInterfaceBasePtr() : PyInterfaceBasePtr(new PyCollisionCheckerBase(pCollisionChecker,pyenv));
}

CollisionReportPtr GetCollisionReport(object o)
{
    if( IS_PYTHONOBJECT_NONE(o) ) {
        return CollisionReportPtr();
    }

    try {
        PyCollisionReportPtr pyreport = o.cast<PyCollisionReportPtr>();
        if(pyreport != nullptr) {
            return pyreport->report;
        }
    }
    catch(...) {}
    return CollisionReportPtr();
}

CollisionReportPtr GetCollisionReport(PyCollisionReportPtr p)
{
    return !p ? CollisionReportPtr() : p->report;
}

PyCollisionReportPtr toPyCollisionReport(CollisionReportPtr p, PyEnvironmentBasePtr pyenv)
{
    if( !p ) {
        return PyCollisionReportPtr();
    }
    PyCollisionReportPtr pyreport(new PyCollisionReport(p));
    pyreport->init(pyenv);
    return pyreport;
}

void UpdateCollisionReport(PyCollisionReportPtr p, PyEnvironmentBasePtr pyenv)
{
    if( !!p ) {
        p->init(pyenv);
    }
}

void UpdateCollisionReport(object o, PyEnvironmentBasePtr pyenv)
{
    try {
        PyCollisionReportPtr pyreport = o.cast<PyCollisionReportPtr>();
        if(pyreport != nullptr) {
            UpdateCollisionReport(pyreport, pyenv);
        }
    }
    catch(...) {}
}

PyCollisionCheckerBasePtr RaveCreateCollisionChecker(PyEnvironmentBasePtr pyenv, const std::string& name)
{
    CollisionCheckerBasePtr p = OpenRAVE::RaveCreateCollisionChecker(GetEnvironment(pyenv), name);
    if( !p ) {
        return PyCollisionCheckerBasePtr();
    }
    return PyCollisionCheckerBasePtr(new PyCollisionCheckerBase(p,pyenv));
}

void init_openravepy_collisionchecker(py::module& m)
{
    py::enum_<CollisionOptions>(m, "CollisionOptions" DOXY_ENUM(CollisionOptions))
    .value("Distance",CO_Distance)
    .value("UseTolerance",CO_UseTolerance)
    .value("Contacts",CO_Contacts)
    .value("RayAnyHit",CO_RayAnyHit)
    .value("ActiveDOFs",CO_ActiveDOFs)
    .value("AllLinkCollisions", CO_AllLinkCollisions)
    .value("AllGeometryContacts", CO_AllGeometryContacts)
    ;
    py::enum_<CollisionAction>(m, "CollisionAction" DOXY_ENUM(CollisionAction))
    .value("DefaultAction",CA_DefaultAction)
    .value("Ignore",CA_Ignore)
    ;

    py::class_<PyCollisionReport::PYCONTACT, OPENRAVE_SHARED_PTR<PyCollisionReport::PYCONTACT> >(m, "Contact", DOXY_CLASS(CollisionReport::CONTACT))
    .def_readonly("pos",&PyCollisionReport::PYCONTACT::pos)
    .def_readonly("norm",&PyCollisionReport::PYCONTACT::norm)
    .def_readonly("depth",&PyCollisionReport::PYCONTACT::depth)
    .def("__str__",&PyCollisionReport::PYCONTACT::__str__)
    .def("__unicode__",&PyCollisionReport::PYCONTACT::__unicode__)
    ;
    py::class_<PyCollisionReport, OPENRAVE_SHARED_PTR<PyCollisionReport> >(m, "CollisionReport", DOXY_CLASS(CollisionReport))
    .def_readonly("options",&PyCollisionReport::options)
    .def_readonly("plink1",&PyCollisionReport::plink1)
    .def_readonly("plink2",&PyCollisionReport::plink2)
    .def_readonly("minDistance",&PyCollisionReport::minDistance)
    .def_readonly("numWithinTol",&PyCollisionReport::numWithinTol)
    .def_readonly("contacts",&PyCollisionReport::contacts)
    .def_readonly("vLinkColliding",&PyCollisionReport::vLinkColliding)
    .def_readonly("nKeepPrevious", &PyCollisionReport::nKeepPrevious)
    .def("__str__",&PyCollisionReport::__str__)
    .def("__unicode__",&PyCollisionReport::__unicode__)
    ;

    bool (PyCollisionCheckerBase::*pcolb)(PyKinBodyPtr) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcolbr)(PyKinBodyPtr, PyCollisionReportPtr) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcolbb)(PyKinBodyPtr,PyKinBodyPtr) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcolbbr)(PyKinBodyPtr, PyKinBodyPtr,PyCollisionReportPtr) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcoll)(object) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcollr)(object, PyCollisionReportPtr) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcolll)(object,object) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcolllr)(object,object, PyCollisionReportPtr) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcollb)(object, PyKinBodyPtr) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcollbr)(object, PyKinBodyPtr, PyCollisionReportPtr) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcolle)(object,object,object) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcoller)(object, object,object,PyCollisionReportPtr) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcolbe)(PyKinBodyPtr,object,object) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcolber)(PyKinBodyPtr, object,object,PyCollisionReportPtr) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcolyb)(OPENRAVE_SHARED_PTR<PyRay>,PyKinBodyPtr) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcolybr)(OPENRAVE_SHARED_PTR<PyRay>, PyKinBodyPtr, PyCollisionReportPtr) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcoly)(OPENRAVE_SHARED_PTR<PyRay>) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcolyr)(OPENRAVE_SHARED_PTR<PyRay>, PyCollisionReportPtr) = &PyCollisionCheckerBase::CheckCollision;
    bool (PyCollisionCheckerBase::*pcoltbr)(object, PyKinBodyPtr, PyCollisionReportPtr) = &PyCollisionCheckerBase::CheckCollisionTriMesh;
    bool (PyCollisionCheckerBase::*pcolter)(object, PyCollisionReportPtr) = &PyCollisionCheckerBase::CheckCollisionTriMesh;
    bool (PyCollisionCheckerBase::*pcolobb)(object, object, PyCollisionReportPtr) = &PyCollisionCheckerBase::CheckCollisionOBB;

    py::class_<PyCollisionCheckerBase, OPENRAVE_SHARED_PTR<PyCollisionCheckerBase>, PyInterfaceBase >(m, "CollisionChecker", DOXY_CLASS(CollisionCheckerBase))
    .def("InitEnvironment", &PyCollisionCheckerBase::InitEnvironment, DOXY_FN(CollisionCheckerBase, InitEnvironment))
    .def("DestroyEnvironment", &PyCollisionCheckerBase::DestroyEnvironment, DOXY_FN(CollisionCheckerBase, DestroyEnvironment))
    .def("InitKinBody", &PyCollisionCheckerBase::InitKinBody, DOXY_FN(CollisionCheckerBase, InitKinBody))
    .def("RemoveKinBody", &PyCollisionCheckerBase::RemoveKinBody, DOXY_FN(CollisionCheckerBase, RemoveKinBody))
    .def("SetGeometryGroup", &PyCollisionCheckerBase::SetGeometryGroup, DOXY_FN(CollisionCheckerBase, SetGeometryGroup))
    .def("SetBodyGeometryGroup", &PyCollisionCheckerBase::SetBodyGeometryGroup, py::arg("body"), py::arg("groupname"), DOXY_FN(CollisionCheckerBase, SetBodyGeometryGroup))
    .def("GetGeometryGroup", &PyCollisionCheckerBase::GetGeometryGroup, DOXY_FN(CollisionCheckerBase, GetGeometryGroup))
    .def("SetCollisionOptions",&PyCollisionCheckerBase::SetCollisionOptions, DOXY_FN(CollisionCheckerBase,SetCollisionOptions "int"))
    .def("GetCollisionOptions",&PyCollisionCheckerBase::GetCollisionOptions, DOXY_FN(CollisionCheckerBase,GetCollisionOptions))
    .def("CheckCollision",pcolb,py::arg("body"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBodyConstPtr; CollisionReportPtr"))
    .def("CheckCollision",pcolbr,py::arg("body"), py::arg("report"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBodyConstPtr; CollisionReportPtr"))
    .def("CheckCollision",pcolbb,py::arg("body1"), py::arg("body2"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBodyConstPtr; KinBodyConstPtr; CollisionReportPtr"))
    .def("CheckCollision",pcolbbr,py::arg("body1"), py::arg("body2"), py::arg("report"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBodyConstPtr; KinBodyConstPtr; CollisionReportPtr"))
    .def("CheckCollision",pcoll,py::arg("link"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBody::LinkConstPtr; CollisionReportPtr"))
    .def("CheckCollision",pcollr,py::arg("link"), py::arg("report"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBody::LinkConstPtr; CollisionReportPtr"))
    .def("CheckCollision",pcolll,py::arg("link1"), py::arg("link2"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBody::LinkConstPtr; KinBody::LinkConstPtr; CollisionReportPtr"))
    .def("CheckCollision",pcolllr,py::arg("link1"), py::arg("link2"), py::arg("report"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBody::LinkConstPtr; KinBody::LinkConstPtr; CollisionReportPtr"))
    .def("CheckCollision",pcollb,py::arg("link"), py::arg("body"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBody::LinkConstPtr; KinBodyConstPtr; CollisionReportPtr"))
    .def("CheckCollision",pcollbr,py::arg("link"), py::arg("body"), py::arg("report"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBody::LinkConstPtr; KinBodyConstPtr; CollisionReportPtr"))
    .def("CheckCollision",pcolle,py::arg("link"), py::arg("bodyexcluded"), py::arg("linkexcluded"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBody::LinkConstPtr; const std::vector; const std::vector; CollisionReportPtr"))
    .def("CheckCollision",pcoller,py::arg("link"), py::arg("bodyexcluded"), py::arg("linkexcluded"), py::arg("report"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBody::LinkConstPtr; const std::vector; const std::vector; CollisionReportPtr"))
    .def("CheckCollision",pcolbe,py::arg("body"), py::arg("bodyexcluded"), py::arg("linkexcluded"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBodyConstPtr; const std::vector; const std::vector; CollisionReportPtr"))
    .def("CheckCollision",pcolber,py::arg("body"), py::arg("bodyexcluded"), py::arg("linkexcluded"), py::arg("report"), DOXY_FN(CollisionCheckerBase,CheckCollision "KinBodyConstPtr; const std::vector; const std::vector; CollisionReportPtr"))
    .def("CheckCollision",pcolyb,py::arg("ray"), py::arg("body"), DOXY_FN(CollisionCheckerBase,CheckCollision "const RAY; KinBodyConstPtr; CollisionReportPtr"))
    .def("CheckCollision",pcolybr,py::arg("ray"), py::arg("body"), py::arg("report"), DOXY_FN(CollisionCheckerBase,CheckCollision "const RAY; KinBodyConstPtr; CollisionReportPtr"))
    .def("CheckCollision",pcoly,py::arg("ray"), DOXY_FN(CollisionCheckerBase,CheckCollision "const RAY; CollisionReportPtr"))
    .def("CheckCollision",pcolyr,py::arg("ray"), py::arg("report"), DOXY_FN(CollisionCheckerBase,CheckCollision "const RAY; CollisionReportPtr"))
    .def("CheckCollisionTriMesh",pcolter,py::arg("trimesh"), py::arg("report"), DOXY_FN(CollisionCheckerBase,CheckCollision "const TriMesh; CollisionReportPtr"))
    .def("CheckCollisionTriMesh",pcoltbr,py::arg("trimesh"), py::arg("body"), py::arg("report"), DOXY_FN(CollisionCheckerBase,CheckCollision "const TriMesh; KinBodyConstPtr; CollisionReportPtr"))
    .def("CheckCollisionOBB", pcolobb, py::arg("aabb"), py::arg("pose"), py::arg("report"), DOXY_FN(CollisionCheckerBase,CheckCollision "const AABB; const Transform; CollisionReport"))
    .def("CheckSelfCollision",&PyCollisionCheckerBase::CheckSelfCollision,py::arg("linkbody"), py::arg("report"), DOXY_FN(CollisionCheckerBase,CheckSelfCollision "KinBodyConstPtr, CollisionReportPtr"))
    .def("CheckCollisionRays",&PyCollisionCheckerBase::CheckCollisionRays,
         py::arg("rays"), py::arg("body"), py::arg("front_facing_only"),
                                      "Check if any rays hit the body and returns their contact points along with a vector specifying if a collision occured or not. Rays is a Nx6 array, first 3 columns are position, last 3 are direction*range. The return value is: (N array of hit points, Nx6 array of hit position and surface normals.")
    ;

    m.def("RaveCreateCollisionChecker", openravepy::RaveCreateCollisionChecker, py::arg("env"), py::arg("name"), DOXY_FN1(RaveCreateCollisionChecker));
}

}
