import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import netCDF4
from mpl_toolkits.basemap import Basemap
from matplotlib.pyplot import cm
#import colormaps

url = 'cut.nc'
nc = netCDF4.Dataset(url)
#list variables
nc.variables.keys()
lon_rho = nc.variables['lon_rho'][:][:]
lat_rho = nc.variables['lat_rho'][:][:]
zeta = nc.variables['h'][:,:]
#land_mask = 1 - nc.variables['coastline_mask'][:,:]


#mpl.rcParams['axes.labelsize'] = 8.
#mpl.rcParams['xtick.labelsize'] = 6.
#mpl.rcParams['ytick.labelsize'] = 6.





#basemap.fillcontinents(color='black',lake_color='black')
#basemap.drawcountries()
#basemap.drawstates()

#x_rho, y_rho = basemap(lon_rho,lat_rho)
#h1 = basemap.pcolormesh(x_rho, y_rho, tsl, vmin=-0.2, vmax=0.4, cmap='gnuplot')
#basemap.shadedrelief()


# draw parallels.
#parallels = np.arange(-24.5,-14.5,1.0)
#basemap.drawparallels(parallels,labels=[1,0,0,0],fontsize=14)
# draw meridians
#meridians = np.arange(113.0,124.0,1.0)
#basemap.drawmeridians(meridians,labels=[0,0,0,1],fontsize=14)
# add colorbar.
#cbar = basemap.colorbar(location='bottom',pad="10%")
#cbar.ax.tick_params(labelsize=14)
#cbar.set_label('Sea Surface Height Anomaly (metres)', fontsize=22, weight='bold')
#plt.legend( loc='upper right' )
#x,y = basemap(117.46176702, -20.7102136158)
#plt.plot(x,y,marker='*',markersize=20,color='orange')
#plt.figtext(0.98, 0.02, '117.46E, 20.71S', horizontalalignment='right')

#cbar = basemap.colorbar(location='bottom',pad="10%")
#
#cbar.set_label('Sea Surface Height Anomaly (metres)', fontsize=22, weight='bold')

print lat_rho.min()-1
print lat_rho.max()+1
print lon_rho.min()-1
print lon_rho.max()+1


for i in range(0,1):
    fig1 = plt.figure(frameon=False, figsize=(15,10))
    ax = fig1.add_subplot(111)

    ax.axis('off')

    #mpl.rcParams['font.size'] = 10.
    #mpl.rcParams['font.family'] = 'sans-serif'
    #mpl.rcParams['font.sans-serif'] = 'Helvetica'

    basemap = Basemap(projection='merc',llcrnrlat=lat_rho.min()-1,urcrnrlat=lat_rho.max()+1,llcrnrlon=lon_rho.min()-1,urcrnrlon=lon_rho.max()+1,resolution='h')




    colorMap=mpl.cm.Paired
    nlevs = 256
    clevs = np.linspace(2,4765,num=nlevs)

    basemap.drawmapboundary(fill_color='#FFFFFF')
    basemap.fillcontinents(color='#000000',lake_color='#000000')


    cax = basemap.contourf(lon_rho,lat_rho,(zeta[:,:]),levels=clevs, cmap=colorMap, latlon=True)

    #cbar = fig1.colorbar(cax)
    #cbar.ax.tick_params(labelsize=14)

    data = np.loadtxt('ita.txt',dtype=np.str,delimiter=' ')
    color=iter(cm.gist_ncar(np.linspace(0,1,1001)))
    for j in range(0, 1000):
        start = (j*72)
        print data[start,0]
        end = start+72
        #print data[end,0]
        latitude = data[start:end,1].astype(np.float)
        longitude = data[start:end,2].astype(np.float)
        x, y = basemap(longitude,latitude)
        c=next(color)
        label = "Ensemble member %d" % (j+1)
        plt.plot(x,y,'o-',c=c,linewidth=2, label=label)


    fname = "roms_cut_%02d" % i
    fig1.savefig(fname)

#plt.pcolormesh(lon_rho,lat_rho,ma.masked_where(land_mask,tsl),vmin=-1,vmax=1)
#plt.colorbar()
