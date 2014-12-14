from ROOT import TFile, TH1F
import matplotlib.pyplot as plt
import numpy as np

#-----------------------------------------------------------------------------------------------------------------------
def TH1Farray(h1):
    # unpack a TH1F into x and y array
    nb = h1.GetNbinsX()
    x  = np.zeros(nb)
    y  = np.zeros(nb)
    for i in range (0,nb):
        x[i] = h1.GetBinCenter(i)
        y[i] = h1.GetBinContent(i)

    return x,y

#-----------------------------------------------------------------------------------------------------------------------

def plotGamma(fname, material, savefig):
    #
    # Input
    #       fname    = GEANT4 output file
    #       material = Material name
    #
    f = TFile(fname)
    # get the data from 1D histograms
    E,PE    = TH1Farray(f.FindObjectAny(material+"_phot"))
    _,Compt = TH1Farray(f.FindObjectAny(material+"_compt"))
    _,Conv  = TH1Farray(f.FindObjectAny(material+"_conv"))
    # plot histogram
    p=plt.semilogy(E,PE,color='red',linestyle='--')
    fig = plt.gcf()
    fig.set_size_inches(10, 5)
    plt.semilogy(E,Compt,color='blue',linestyle='-.')
    plt.semilogy(E,Conv,color='green',linestyle='-.')
    plt.semilogy(E,PE+Compt+Conv,color='black')
    # title of plot
    plt.title("Material = "+material+". G4LEDATA = "+f.FindObjectAny("G4LEDATA").GetTitle(),fontsize=14)
    # set axis ranges
    ax = plt.gca()
    ax.set_xlim([-3,1.3])
    ax.set_ylim([1e-4,5e4])
    # set axis title
    ax.set_xlabel('$^{10} \\log (E/MeV)$',fontsize=16)
    ax.set_ylabel('$\\mu_{\\rho}(cm^2/g)$',fontsize=16)
    #
    lgd = plt.legend((["Photo Electric","Compton","Conversion","Total"]), bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)

    if savefig:
        fig = plt.gcf()
        pname = material + "_gamma.pdf"
        fig.savefig(pname, bbox_extra_artists=(lgd,), bbox_inches='tight')
#-----------------------------------------------------------------------------------------------------------------------
def plotNeutron(fname, material, savefig):
    #
    # Input
    #       fname    = GEANT4 output file
    #       material = Material name
    #
    f = TFile(fname)
    # get the data from 1D histograms
    E,inelastic = TH1Farray(f.FindObjectAny(material+"_neutron_inelastic"))
    _,elastic   = TH1Farray(f.FindObjectAny(material+"_neutron_elastic"))
    _,capture   = TH1Farray(f.FindObjectAny(material+"_neutron_capture"))
    # plot histogram

    p=plt.semilogy(E,inelastic,color='red',linestyle='--')
    fig = plt.gcf()
    fig.set_size_inches(10, 5)
    plt.semilogy(E,elastic,color='blue',linestyle='-.')
    plt.semilogy(E,capture,color='green',linestyle='-.')
    plt.semilogy(E,inelastic+elastic+capture,color='black')
    # title of plot
    plt.title("Material = "+material+". Hadronic model = "+f.FindObjectAny("HADmodel").GetTitle(),fontsize=14)
    # set axis ranges
    ax = plt.gca()
    ax.set_xlim([-8,1.])
    #ax.set_ylim([1e-4,5e4])
    # set axis title
    ax.set_xlabel('$^{10} \\log (E/MeV)$',fontsize=16)
    ax.set_ylabel('$\\mu (cm^{-1})$',fontsize=16)
    #
    lgd = plt.legend((["Inelastic scatter","Elastic scatter","Capture","Total"]), bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)

    if savefig:
       fig = plt.gcf()
       pname = material + "_neutron.pdf"
       fig.savefig(pname, bbox_extra_artists=(lgd,), bbox_inches='tight')

#-----------------------------------------------------------------------------------------------------------------------
def getMaterials(fname):
    f = TFile(fname)
    gdir = f.GetDirectory("physics/electromagnetic")
    materials = []
    for key in gdir.GetListOfKeys():
        parname = key.GetName()
        obj = gdir.FindObjectAny(parname)
        if 'TH1F' in str(obj):
            hname = obj.GetName()
            mname = str.split(hname,'_')
            mat = mname[0]
            for i in range(1,len(mname)-1):
                mat = mat + '_' + mname[i]
            materials.append(mat)

    return list(set(materials))