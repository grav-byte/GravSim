G_SI = 6.67430e-11;
AU_scale = 1.496e11;           # meters
year = 3.154e7 * .2;           # seconds
mEarth_SI = 5.972e24;    # kg
m_sun = 1.9891e30 #kg

mEarth_norm = G_SI * mEarth_SI * year*year / (AU_scale*AU_scale*AU_scale)
msun_norm = G_SI * m_sun * year*year / (AU_scale*AU_scale*AU_scale)

v_earth = 29.8e3 # m/s

vEarth_norm = v_earth * year / AU_scale


